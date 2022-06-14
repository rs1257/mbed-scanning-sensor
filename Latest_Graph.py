
import pygame #pygame libraries
from pygame.locals import *
import serial #getting data from mbed
import sys,os #exiting graph
import math #maths calculations
#latest
##############################################
#                    MAIN                    #
##############################################
def Serialdata(mbed):
	global ir_raw, us_raw, ir, us, max_rot, min_rot, cal_point, sweep_num, checker, line, currentline, count, angle, modecheck, newmulti
	mbed.open()
	x = mbed.readline()
	vals = []
	vals = x.split(';', 16)
	vals = vals[1:13]
	#print vals
	if len(vals) == 12:
		for val in vals:
			if val == '' or val == None or val == "\x00" or val == "\xff":
				val = 0
		ir_raw = int(vals[0])
		us_raw = int(vals[1])
		ir = int(vals[2])
		us = int(vals[3])
		angle = int(vals[4])
		max_rot = int(vals[5])
		min_rot = int(vals[6])
		cal_point = int(vals[7])
		sweep_num = int(vals[8])
		if vals[9] == '\n':
			checker = 0
		else:	
			checker = int(vals[9])
		if vals[10] == '\n':
			modecheck = 0
		else:
			modecheck = int(vals[10])
		line += 1	
		newmulti = int(vals[11])
	mbed.close()
 
def main():
	global mode, ir, angle, sweep_num,tick, modecheck, status
	mbed = serial.Serial()
	mbed.port = "/dev/ttyACM0"
	mbed.bytesize = serial.EIGHTBITS
	mbed.parity = serial.PARITY_NONE
	mbed.stopbits = serial.STOPBITS_ONE
	done = False
	while not done:
		Serialdata(mbed)
		#readfile()
		key = pygame.key.get_pressed()
		for event in pygame.event.get():
			if event.type == QUIT or key[K_ESCAPE]:
				done = True
				pygame.display.quit()
				pygame.quit()
		if mode == "tape":  
			draw_tapemeasureplot()  
		elif mode == "overhead":
			draw_overheadplot()
		elif mode == "radar":
			draw_radarplot()
		elif mode == "multi" and modecheck == 3:
			screen.fill([0,0,0])
			testmode()
			draw_multiviewplot()  
		elif mode == "multi":
			screen.fill([0,0,0])
			testmode()
			draw_multiviewplot()  
		if status == True:
			cleararray()
			status = False	 
		
		pygame.display.flip() #update screen needs to be outside loop or will only update on click        
		pygame.event.clear()
		clock.tick(tick)	
		
##############################################
#                TAPEMEASURE                 #
##############################################
def draw_tapemeasureplot():  #tape measure mode 
	global font,us_raw, ir_raw, ir, us, screen, cal_point,calpoints, previous, ccount, modecheck
	screen.fill([0,0,0])
	if previous == "tape":
		pass
	else:
		s = pygame.Surface((0,150))	
		z = pygame.Surface((0,150))	
		previous = "tape"

	irdistance = ir*7
	usdistance = us*7
	tape_buttons()
	 
	point = cal_point
	step = 7
	pygame.display.set_caption("Tape Measure Mode")
	if irdistance <= 0: #draws graph grid to screen
		s = pygame.Surface((0,150))
	elif irdistance > 700: 
		s = pygame.Surface((700,150))
	else:
		s = pygame.Surface((irdistance,150)) 
	if usdistance <= 0: #draws graph grid to screen
		z = pygame.Surface((0,150))
	elif usdistance > 700: 
		z = pygame.Surface((700,150))
	else:
		z = pygame.Surface((usdistance,150))  	   
	s.fill((50,50,200))
	z.fill((50,200,50))
	screen.blit(z,(50,200))
	screen.blit(s,(50,50))
	q = 110
	y = 10
	if point != 0:	
		calpoints.append([point,step])
	for val in calpoints:
		set_calibrationpoints(val[0],val[1])
	numbers = font.render(str(0), True, colour)
	screen.blit(numbers, (45,25))
	numbers = font.render(str(100), True, colour)
	screen.blit(numbers, (725,25))
	ccount += 1

	for x in range(9):
		numbers = font.render(str(y), True, colour)
		screen.blit(numbers, (q-3,25))
		q += 70
		y += 10
	x = 85
	count = 1
	pygame.draw.rect(screen ,colour, (50,50,700,300), 4) #draws bargraph to screen
	while x < 735:   
		if (count % 2 == 0):
			pygame.draw.line(screen, colour, (x, 50), (x,350),3)
		else:    
			pygame.draw.line(screen, colour, (x, 50), (x,350),1)
		x += 35
		count += 1		

def tape_buttons(): #check poisiton values so in middle of boxes
	global mode,buttonfont, datafont, ir_raw, us_raw, ir, us
   
	mouse = pygame.mouse.get_pos()
	click = pygame.mouse.get_pressed()    
	button(5,150,40,40,178,102,255,204,153,255,"overhead","OVER",11,165,mouse[0],mouse[1],click[0],buttonfont)
	button(5,100,40,40,255,255,0,255,255,204,"tape","TAPE",11,115,mouse[0],mouse[1],click[0],buttonfont)
	button(5,200,40,40,0,255,128,153,255,204,"radar","RADAR",8,215,mouse[0],mouse[1],click[0],buttonfont)
	button(5,250,40,40,255,102,102,255,153,153,"multi","MULTI",10,265,mouse[0],mouse[1],click[0],buttonfont)
	raw_ir = "RAW IR: " + str(ir_raw)
	irdis = "IR: " + str(ir)
	data = font.render(raw_ir, True, (50,50,200))
	screen.blit(data, (50,3))
	data = font.render(irdis, True, (50,50,200))
	screen.blit(data, (500,3))
	raw_us = "RAW US: " + str(us_raw)
	usdis = "US: " + str(us)
	data = font.render(raw_us, True, (50,200,50))
	screen.blit(data, (250,3))
	data = font.render(usdis, True, (50,200,50))
	screen.blit(data, (650,3))

def set_calibrationpoints(points,step): #choose what the calibration points are
			pygame.draw.polygon(screen, (0,255,0), ((45+(points*step), 370), (55+(points*step), 370), (55+(points*step), 360), (60+(points*step), 360), (50+(points*step), 350), (40+(points*step), 360), (45+(points*step), 360))) 

##############################################
#                  OVERHEAD                  #
##############################################
def draw_overheadplot(): #overhead plot
	global us, ir,ir_raw,us_raw, a, sensor, screen, pointlist, no_points,previousmode, previous, t# need to set points list to zero after mode changes
	pygame.display.set_caption("Overhead Mode") #set title
	screen.fill([0,0,0]) #clear point list
	if previous == "overhead":
		pass
	else:
		pointlist = []
		a = 0
		previous = "overhead"
	
	if sensor == "us":
		if previousmode == "us":
			pass
		elif previousmode == "ir":
			pointlist = []
			a = 0
		z = us * 2
		display_rawus()
		previousmode = "us" 
	elif sensor == "ir":
		if previousmode == "ir":
			pass
		elif previousmode == "us":
			pointlist = []
			a = 0
		z = ir * 2
		display_rawir()
		previousmode = "ir" 
	x = 50
	y = 50
	while x < 770: #overhead plot
		pygame.draw.line(screen, (0,51,102), (x,50),(x,350),1 )
		x+=20
	while y < 370:
		pygame.draw.line(screen, (0,51,102), (50,y),(750,y),1 )
		y+=20
	draw_buttons()	 
	endofsweep()
	if a >= 700: #catches out of bounds
		a = 700
	if z < 0:
		z = 0 
	elif z >= 300:
		z = 300  
	elif a <= 0:
		a = 0
	if z < 0 :
		z = 0 
	elif z >= 300:
		z = 300           
	elif z < 0:
		z = 0
	if a >= 700:
		a = 700
	elif a <= 0:
		a = 0    
	elif z >= 300:
		z = 300
	if a >= 700:
		a = 700
	elif a <= 0:
		a = 0	 
	if len(pointlist) >= no_points:
		pointlist.pop(0) #if list is full on graph it shifts it along 1
		pointlist.append([(a+50),((-1*z)+350)]) #350 for normal
		for val in pointlist:
			val[0] -= (700/no_points)	
	else:
		pointlist.append([(a+50),((-1*z)+350)]) #350 for normal '''change boundaries'''
	for val in pointlist:        
		plot_point(val[0],val[1])
	a += (700/no_points)
	if len(pointlist) == 1:
		pass
	else: 
		draw_lines(pointlist)
	

##############################################
#                   RADAR                    #
##############################################
def draw_radarplot(): #radar mode
	global angle, max_rot, min_rot, us, ir, us_raw, ir_raw, sensor, mode, rpointlist,previousmode, rcount, next, previous
	screen.fill([0,0,0]) #clear point list
	pygame.display.set_caption("Radar Mode")
	radarangle = angle
	if previous == "radar":
		pass
	else:
		rpointlist = []	
		previous = "radar"
	if sensor == "ir":
		if previousmode == "ir":
			pass
		elif previousmode == "us":
			rpointlist = []
		distance = ir * 3
		display_rawir()
		previousmode = "ir"
	elif sensor == "us":
		if previousmode == "us":
			pass
		elif previousmode == "ir":
			rpointlist = []
		distance = us * 3
		display_rawus()
		previousmode = "us"    
	z = 10
	pygame.draw.line(screen, (0,51,102), (400, 400), (400, 30), 2)
	pygame.draw.line(screen, (0,51,102), (30, 400), (770, 400), 8)
	pygame.draw.line(screen, (0,51,102), (400, 400), (400 - (370*math.cos(math.radians(45))),400 - (370*math.sin(math.radians(45)))), 2)
	pygame.draw.line(screen, (0,51,102), (400, 400), (400 - (370*math.cos(math.radians(135))),400 - (370*math.sin(math.radians(135)))),2)
	count = 0 #changes size of lines every two
	endofsweep()
	while z < 380: #radius = 380
		if (count % 2 == 0):  
			pygame.draw.circle(screen, (0,128,255), (400, 400), z, 2)
		else:
			pygame.draw.circle(screen, (0,51,102), (400, 400), z, 1)    
		z += 20
		count += 1
	draw_buttons()
	if distance >= 370: #catches out of bounds
		distance = 370
		if radarangle <= 180:
			radarangle = 0
		elif radarangle >= 360:
			radarangle = 180  
	elif distance <= 0:
		distance = 0
		if radarangle <= 180:
			radarangle = 0
		elif radarangle >= 360:
			radarangle = 180  
	elif radarangle <= 180:
		radarangle = 0
	elif radarangle >= 360:
		radarangle = 180        
	radarangle = angle + 180
	p = plotoncircle(400,400,distance,radarangle)
	rpointlist.append(p)        
	if len(rpointlist) <= 1:
		pass
	else:
		#for val in rpointlist:
			#plot_point(val[0],val[1])
		draw_lines(rpointlist) 	        

def plotoncircle(xcenter, ycenter, distance, angle): #math for moving around a circle
	angle = math.radians(angle) #convert degrees to radians
	x0 = xcenter #sets center need to translate to
	y0 = ycenter
	x = x0 + (distance * math.cos(angle)) #trig on the point
	y = y0 + (distance * math.sin(angle))
	plot_point(x,y)
	return [x,y]
			
##############################################
#                 MULTIVIEW                  #
##############################################
def draw_multiviewplot(): #multiview mode '''maybe plot all the sweeps at the same time'''
	global mode, sensor, ir, us, us_raw, ir_raw, max_rot, min_rot, sweep_num, angle, p0, p1, p2, p3, mcount, previous, distance, checker, quad1, quad2, quad3, quad4, scan1, scan2, scan3, scan4, count
	screen.fill([0,0,0]) #clear point list
	if previous == "multi":
		pass
	else:
		p0 = []
		p1 = []
		p2 = []
		p3 = []	
		previous = "multi"
	multi_buttons()
	pygame.display.set_caption("Multiview Mode")
	pygame.draw.line(screen, (0,51,102), (400,40), (400,360),3)
	pygame.draw.line(screen, (0,51,102), (560,200), (240,200),3)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 170, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 150, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 125, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 100, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 75, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 50, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 25, 1) 
	p0 = []
	p1 = []
	p2 = []
	p3 = []		
	for val in quad1:
		p0.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in quad2:
		p1.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in quad3:
		p2.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in quad4:
		p3.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in p0:        
		plot_point(val[0],val[1])
	if len(p0) <= 1:
		pass
	else: 
		draw_lines(p0)

	for val in p1:        
		plot_point(val[0],val[1])
	if len(p1) <= 1:
		pass
	else: 
		draw_lines(p1)

	for val in p2:        
		plot_point(val[0],val[1])
	if len(p2) <= 1:
		pass
	else: 
		draw_lines(p2)

	for val in p3:        
		plot_point(val[0],val[1])
	if len(p3) <= 1:
		pass
	else: 
		draw_lines(p3)		

	 
def cutoff():
	global angle, checker, distance, ir, us, limit

	if (ir < 10):
		distance = us * 2
	else:
		distance = ((ir + us)/2.0) * 2

	if distance > limit:
		distance = limit
	elif distance < 0:
		distance = 0
	store(distance, angle, checker)

def flip():
	global scan1, scan2, scan3, scan4, maximum1, maximum2, maximum3, maximum4, flipped1, flipped2, flipped3, flipped4, p0, p1, p2, p3
	difflist1 = []
	difflist2 = []
	difflist3 = []
	difflist4 = []
	flipped1 = []
	flipped2 = []
	flipped3 = []
	flipped4 = []
	screen.fill([0,0,0])
	pygame.display.set_caption("Multiview Mode")
	multi_buttons()
	pygame.draw.line(screen, (0,51,102), (400,40), (400,360),3)
	pygame.draw.line(screen, (0,51,102), (560,200), (240,200),3)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 170, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 150, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 125, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 100, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 75, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 50, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 25, 1)    
	if scan1 != []:
		for val in scan1:
			if val[1] > maximum1[1]:
				maximum1 = val

	if scan2 != []:
		for val in scan2:
			if val[1] > maximum2[1]:
				maximum2 = val
	if scan3 != []:
		for val in scan3:
			if val[1] > maximum3[1]:
				maximum3 = val
	if scan4 != []:
		for val in scan4:
			if val[1] > maximum4[1]:
				maximum4 = val	
	for val in scan1:
		diff = maximum1[1] - val[1]
		difflist1.append([val[0], diff])
	for val in scan2:
		diff = maximum2[1] - val[1]
		difflist2.append([val[0], diff])
	for val in scan3:
		diff = maximum3[1] - val[1]
		difflist3.append([val[0], diff])
	for val in scan4:
		diff = maximum4[1] - val[1]
		difflist4.append([val[0], diff])
	
	difflist1 = sorted(difflist1)
	difflist2 = sorted(difflist2)
	difflist3 = sorted(difflist3)
	difflist4 = sorted(difflist4)

	for val in difflist1:
		omega = maximum1[1] + val[1]												
		flipped1.append([val[0], omega])
	for val in difflist2:
		omega = maximum2[1] + val[1]												
		flipped2.append([val[0], omega])
	for val in difflist3:
		omega = maximum3[1] + val[1]												
		flipped3.append([val[0], omega])
	for val in difflist4:
		omega = maximum4[1] + val[1]												
		flipped4.append([val[0], omega])

	p0 = []
	p1 = []
	p2 = []
	p3 = []
	for val in flipped1:
		p0.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in flipped2:
		p1.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in flipped3:
		p2.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in flipped4:
		p3.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in p0:        
		plot_point(val[0],val[1])
	if len(p0) <= 1:
		pass
	else: 
		draw_lines(p0)

	for val in p1:        
		plot_point(val[0],val[1])
	if len(p1) <= 1:
		pass
	else: 
		draw_lines(p1)

	for val in p2:        
		plot_point(val[0],val[1])
	if len(p2) <= 1:
		pass
	else: 
		draw_lines(p2)

	for val in p3:        
		plot_point(val[0],val[1])
	if len(p3) <= 1:
		pass
	else: 
		draw_lines(p3)
	print flipped1
	print flipped2
	print flipped3
	print flipped4	
	pygame.display.flip() 
	removemins()

def removemins():
	global flipped1, flipped2, flipped3, flipped4, maximum1, maximum2, maximum3, maximum4
	temp1 = []
	temp2 = []
	temp3 = []
	temp4 = []
	for val in flipped1:
		if val[1] > maximum1[1]:
			temp1.append(val)
	for val in flipped2:
		if val[1] > maximum2[1]:
			temp2.append(val)
	for val in flipped3:
		if val[1] > maximum3[1]:
			temp3.append(val)
	for val in flipped4:
		if val[1] > maximum4[1]:
			temp4.append(val)
	flipped1 = []
	flipped2 = []
	flipped3 = []
	flipped4 = []
	for val in temp1:
		flipped1.append(val)
	for val in temp2:
		flipped2.append(val)
	for val in temp3:
		flipped3.append(val)
	for val in temp4:
		flipped4.append(val)
	flipped1 = sorted(flipped1)
	flipped2 = sorted(flipped2)
	flipped3 = sorted(flipped3)
	flipped4 = sorted(flipped4)
	print maximum1
	print flipped1
	print maximum2
	print flipped2
	print maximum3
	print flipped3
	print maximum4
	print flipped4

def transform():
	global minimum11, minimum12, minimum21, minimum22, minimum31, minimum32, minimum41, minimum42, flipped1, flipped2, flipped3, flipped4
	for val in flipped1:
		optimusprime = abs(val[1] * math.sin(math.radians(val[0])))
		if optimusprime < minimum11:
			minimum11 = optimusprime
	for val in flipped2:
		optimusprime = abs(val[1] * math.cos(math.radians(val[0])))
		if optimusprime < minimum21:
			minimum21 = optimusprime
	for val in flipped3:
		optimusprime = abs(val[1] * math.sin(math.radians(val[0])))
		if optimusprime < minimum31:
			minimum31 = optimusprime
	for val in flipped4:
		optimusprime = abs(val[1] * math.cos(math.radians(val[0])))
		if optimusprime < minimum41:
			minimum41 = optimusprime

	for val in flipped1:
		optimusprime = abs(val[1] * math.sin(math.radians(val[0])))
		if optimusprime < minimum12 and optimusprime > minimum11:
			minimum12 = optimusprime
	for val in flipped2:
		optimusprime = abs(val[1] * math.cos(math.radians(val[0])))
		if optimusprime < minimum22 and optimusprime > minimum21:
			minimum22 = optimusprime
	for val in flipped3:
		optimusprime = abs(val[1] * math.sin(math.radians(val[0])))
		if optimusprime < minimum32 and optimusprime > minimum31:
			minimum32 = optimusprime
	for val in flipped4:
		optimusprime = abs(val[1] * math.cos(math.radians(val[0])))
		if optimusprime < minimum42 and optimusprime > minimum41:
			minimum42 = optimusprime

	minimum1 = (minimum12 - minimum11) + minimum11
	print "1"
	print minimum1
	minimum2 = (minimum22 - minimum21) + minimum21
	print "2"
	print minimum2
	minimum3 = (minimum32 - minimum31) + minimum31
	print "3"
	print minimum3
	minimum4 = (minimum42 - minimum41) + minimum41
	print "4"
	print minimum4

	screen.fill([0,0,0]) 
	pygame.display.set_caption("Multiview Mode")
	multi_buttons()
	pygame.draw.line(screen, (0,51,102), (400,40), (400,360),3)
	pygame.draw.line(screen, (0,51,102), (560,200), (240,200),3)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 170, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 150, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 125, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 100, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 75, 1)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 50, 2)
	pygame.draw.circle(screen, (0,128,255), (400, 200), 25, 1)  
	p0 = []
	p1 = []
	p2 = []
	p3 = []  
	for val in flipped1:
		p0.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0]))) + minimum1])
	for val in flipped2:
		p1.append([400 - (val[1] * math.cos(math.radians(val[0]))) - minimum2, 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in flipped3:
		p2.append([400 - (val[1] * math.cos(math.radians(val[0]))), 200 - (val[1] * math.sin(math.radians(val[0]))) - minimum3])
	for val in flipped4:
		p3.append([400 - (val[1] * math.cos(math.radians(val[0]))) + minimum4, 200 - (val[1] * math.sin(math.radians(val[0])))])
	for val in p0:        
		plot_point(val[0],val[1])
	if len(p0) <= 1:
		pass
	else: 
		draw_lines(p0)

	for val in p1:        
		plot_point(val[0],val[1])
	if len(p1) <= 1:
		pass
	else: 
		draw_lines(p1)

	for val in p2:        
		plot_point(val[0],val[1])
	if len(p2) <= 1:
		pass
	else: 
		draw_lines(p2)

	for val in p3:        
		plot_point(val[0],val[1])
	if len(p3) <= 1:
		pass
	else: 
		draw_lines(p3)
	pygame.display.flip() 		

def store(distanceavg, angle, checker):
	global quad1, quad2, quad3, quad4, scan1, scan2, scan3, scan4, limit
	if checker == 0: #Sweep Number
		#  ->_|_
		#     |   Scan 1.
		if angle <= 90: #While the first sweep is taking values less than / equal to 90
			c = 0 #0 if no value in vals, 1 if values already been given
			for val in quad4: #for each [angle,distance] array in quad 4
				if angle == val[0]: #if the current angle is equal to the angle in val
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1] + distanceavg)/2.0 #then average the old distance in val with the current distance
						c = 1 #also mark down that there is now a ditance in val.
				else:
					pass #otherwise...
			if c == 0: #if there is no distance in val 
				arr = [angle, distanceavg] #make new entry for quad4
				quad4.append(arr) #put it in quad 4

			if angle == 0: #if angle has returned to 0 then 
				c = 0 #make c 0 again, ready for next run
				for val in quad3:
					if (angle+360) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1] + distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [(angle+360), distanceavg]
					quad3.append(arr)

			if angle == 90:
				c = 0
				for val in quad1:
					if angle == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1] + distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle, distanceavg]
					quad1.append(arr)

		elif angle <= 180:
			c = 0
			for val in quad1:
				if angle == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1] + distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle, distanceavg]
				quad1.append(arr)
			if angle == 180:
				c = 0
				for val in quad2:
					if angle == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else: 
						pass
				if c == 0:
					arr = [angle, distanceavg]
					quad2.append(arr)
		else:
			pass  
		c = 0
		for val in scan1:
			if angle == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					c = 1
				else:
					val[1] = (val[1]+distanceavg)/2.0
					c = 1
		if c == 0:
			arrr = [angle, distanceavg]
			scan1.append(arrr) 
	elif checker == 1:
		if angle <= 90:
			c = 0
			for val in quad4:
				if angle == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle, distanceavg]
				quad4.append(arr)
			if angle == 0:
				c = 0
				for val in quad3:
					if (angle+360) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+360, distanceavg]
					quad3.append(arr)
			if angle == 90:
				c = 0
				for val in quad1:
					if angle == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle, distanceavg]
					quad1.append(arr)
		elif angle <= 180:
			c = 0
			for val in quad1:
				if angle == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle, distanceavg]
				quad1.append(arr)
			if angle == 180:
				c = 0
				for val in quad2:
					if angle == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle, distanceavg]
					quad2.append(arr)
		else:
			pass
		for val in scan1:
			if angle == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					pass
				else:
					val[1] = (val[1]+distanceavg)/2.0
	elif checker == 2:
		if angle <= 90:
			c = 0
			for val in quad1:
				if (angle+90) ==  val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+90, distanceavg]
				quad1.append(arr)
			if angle == 0:
				c = 0
				for val in quad4:
					if (angle+90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+90, distanceavg]
					quad4.append(arr)
			if angle == 90:
				c = 0
				for val in quad2:
					if (angle+90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+90, distanceavg]
					quad2.append(arr)
		elif angle <= 180:
			c = 0
			for val in quad2:
				if (angle+90) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [(angle+90), distanceavg]
				quad2.append(arr)
			if angle == 180:
				c = 0
				for val in quad3:
					if (angle+90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [(angle+90), distanceavg]
					quad3.append(arr)
		else:
			pass		
		c = 0
		for val in scan2:
			if (angle+90) == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					c = 1
				else:
					val[1] = (val[1]+distanceavg)/2.0
					c = 1
		if c == 0:
			arrr = [(angle+90), distanceavg]
			scan2.append(arrr) 
	elif checker == 3:
		if angle <= 90:
			c = 0
			for val in quad1:
				if (angle+90) ==  val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+90, distanceavg]
				quad1.append(arr)
			if angle == 0:
				c = 0
				for val in quad4:
					if (angle+90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+90, distanceavg]
					quad4.append(arr)
			if angle == 90:
				c = 0
				for val in quad2:
					if (angle+90) ==  val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+90, distanceavg]
					quad2.append(arr)
		elif angle <= 180:
			c = 0
			for val in quad2:
				if (angle+90) ==  val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+90, distanceavg]
				quad2.append(arr)
			if angle == 180:
				c = 0
				for val in quad3:
					if (angle+90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+90, distanceavg]
					quad3.append(arr)
		else:
			pass
		for val in scan2:
			if (angle+90) == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					pass
				else:
					val[1] = (val[1]+distanceavg)/2.0
	elif checker == 4:
		if angle <= 90:
			c = 0
			for val in quad2:
				if (angle+180) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+180, distanceavg]
				quad2.append(arr)
			if angle == 0:
				c = 0
				for val in quad1:
					if (angle+180) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+180, distanceavg]
					quad1.append(arr)
			if angle == 90:
				c = 0
				for val in quad3:
					if (angle+180) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+180, distanceavg]
					quad3.append(arr)
		elif angle <= 180:
			c = 0
			for val in quad3:
				if (angle+180) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [(angle+180), distanceavg]
				quad3.append(arr)
			if angle == 180:
				c = 0
				for val in quad4:
					if (angle-180) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+180, distanceavg]
					quad4.append(arr)
		else:
			pass
		c = 0
		for val in scan3:
			if (angle+180) == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					c = 1
				else:
					val[1] = (val[1]+distanceavg)/2.0
					c = 1
		if c == 0:
			arrr = [(angle+180), distanceavg]
			scan3.append(arrr)
	elif checker == 5:
		if angle <= 90:
			c = 0
			for val in quad2:
				if (angle+180) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+180, distanceavg]
				quad2.append(arr)
			if angle == 0:
				c = 0
				for val in quad1:
					if (angle+180) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+180, distanceavg]
					quad1.append(arr)
			if angle == 90:
				c = 0
				for val in quad3:
					if (angle+180) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+180, distanceavg]
					quad3.append(arr)
		elif angle <= 180:
			c = 0            
			for val in quad3:
				if (angle+180) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+180, distanceavg]
				quad3.append(arr)
			if angle == 180:
				c = 0
				for val in quad4:
					if (angle-180) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle-180, distanceavg]
					quad4.append(arr)
		else:
			pass
		for val in scan3:
			if (angle+180) == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					pass
				else:
					val[1] = (val[1]+distanceavg)/2.0
	elif checker == 6:
		if angle <= 90:
			c = 0
			for val in quad3:
				if (angle+270) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+270, distanceavg]
				quad3.append(arr)
			if angle == 0:
				c = 0
				for val in quad2:
					if (angle+270) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+270, distanceavg]
					quad2.append(arr)
			if angle == 90:
				c = 0
				for val in quad4:
					if (angle-90) ==  val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle-90, distanceavg]
					quad4.append(arr)
		elif angle <= 180:
			c = 0
			for val in quad4:
				if (angle-90) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle-90, distanceavg]
				quad4.append(arr)
			if angle == 180:
				c = 0
				for val in quad1:
					if (angle-90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle-90, distanceavg]
					quad1.append(arr)
		else:
			pass
		c = 0
		for val in scan4:
			if (angle+270) == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					c = 1
				else:
					val[1] = (val[1]+distanceavg)/2.0
					c = 1
		if c == 0:
			arrr = [(angle+270), distanceavg]
			scan4.append(arrr)
	elif checker == 7:
		if angle <= 90:
			c = 0
			for val in quad3:
				if (angle+270) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle+270, distanceavg]
				quad3.append(arr)
			if angle == 0:
				c = 0
				for val in quad2:
					if (angle+270) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle+270, distanceavg]
					quad2.append(arr)
			if angle == 90:
				c = 0
				for val in quad4:
					if (angle-90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle-90, distanceavg]
					quad4.append(arr)
		elif angle <= 180:
			c = 0
			for val in quad4:
				if (angle-90) == val[0]:
					if (distanceavg == limit) and (val[1] < limit):
						c = 1
					else:
						val[1] = (val[1]+distanceavg)/2.0
						c = 1
				else:
					pass
			if c == 0:
				arr = [angle-90, distanceavg]
				quad4.append(arr)
			if angle == 180:
				c = 0
				for val in quad1:
					if (angle-90) == val[0]:
						if (distanceavg == limit) and (val[1] < limit):
							c = 1
						else:
							val[1] = (val[1]+distanceavg)/2.0
							c = 1
					else:
						pass
				if c == 0:
					arr = [angle-90, distanceavg]
					quad1.append(arr)
		else:
			pass
		for val in scan4:
			if (angle+270) == val[0]:
				if (distanceavg == limit) and (val[1] < limit):
					pass
				else:
					val[1] = (val[1]+distanceavg)/2.0

	quad1 = sorted(quad1)
	quad2 = sorted(quad2)
	quad3 = sorted(quad3)
	quad4 = sorted(quad4)
	scan1 = sorted(scan1)
	scan2 = sorted(scan2)
	scan3 = sorted(scan3)
	scan4 = sorted(scan4) 

def testmode():
	global distance, quad1, quad2, quad3, quad4, scan1, scan2, scan3, scan4, angle, checker, count, status
	mbed = serial.Serial()
	mbed.port = "/dev/ttyACM0"
	mbed.bytesize = serial.EIGHTBITS
	mbed.parity = serial.PARITY_NONE
	mbed.stopbits = serial.STOPBITS_ONE
	screen.fill([0,0,0])
	cutoff()
	if (checker == 7) and (angle == 9) or (checker == 7) and (angle == 0):
		flip()
		transform()
		while newmulti != 1:
			Serialdata(mbed)	
		cleararray()
		screen.fill([0,0,0])
		return
	
				

def cleararray():
	global quad1, quad2, quad3, quad4, scan1, scan2, scan3, scan4, p0, p1, p2, p3
	quad1 = []
	quad2 = []
	quad3 = []
	quad4 = []
	scan1 = []
	scan2 = []
	scan3 = []
	scan4 = []
	p0 = []
	p1 = []
	p2 = []
	p3 = []

def multi_buttons(): #check poisiton values so in middle of boxes
	global mode,buttonfont, datafont, ir_raw, us_raw, ir, us
   
	mouse = pygame.mouse.get_pos()
	click = pygame.mouse.get_pressed()    
	button(5,150,40,40,178,102,255,204,153,255,"overhead","OVER",11,165,mouse[0],mouse[1],click[0],buttonfont)
	button(5,100,40,40,255,255,0,255,255,204,"tape","TAPE",11,115,mouse[0],mouse[1],click[0],buttonfont)
	button(5,200,40,40,0,255,128,153,255,204,"radar","RADAR",8,215,mouse[0],mouse[1],click[0],buttonfont)
	button(5,250,40,40,255,102,102,255,153,153,"multi","MULTI",10,265,mouse[0],mouse[1],click[0],buttonfont)
	raw_ir = "RAW IR: " + str(ir_raw)
	irdis = "IR: " + str(ir)
	data = font.render(raw_ir, True, (255,255,255))
	screen.blit(data, (50,3))
	data = font.render(irdis, True, (255,255,255))
	screen.blit(data, (500,3))
	raw_us = "RAW US: " + str(us_raw)
	usdis = "US: " + str(us)
	data = font.render(raw_us, True, (255,255,255))
	screen.blit(data, (250,3))
	data = font.render(usdis, True, (255,255,255))
	screen.blit(data, (650,3))

##############################################
#              BUTTONS AND LINES             #
##############################################

def button(x,y,w,h,r1,g1,b1,r2,g2,b2,m,name,yname,xname,mouse0,mouse1,click,font):
	global mode,sensor
	if x+w > mouse0 > x and y+h > mouse1 > y:
		pygame.draw.rect(screen, (r1,g1,b1), (x, y, w, h))
		if click == 1:
			pygame.event.wait()
			if m == "ir" or m == "us":
				sensor = m
			else:
				mode = m
	else:
		pygame.draw.rect(screen, (r2,g2,b2), (x, y, w, h))
	button = font.render(name, True, (0,0,0))
	screen.blit(button, (yname,xname))



def draw_buttons(): #check poisiton values so in middle of boxes
	global mode,buttonfont
   
	mouse = pygame.mouse.get_pos()
	click = pygame.mouse.get_pressed()    
	if mode == "radar": #dont do anything until mouse released
		button(5,10,40,40,178,102,255,204,153,255,"tape","TAPE",10,25,mouse[0],mouse[1],click[0],buttonfont)
		button(5,60,40,40,0,255,128,153,255,204,"overhead","OVER",11,75,mouse[0],mouse[1],click[0],buttonfont)
		button(5,110,40,40,255,102,102,255,153,153,"radar","RADAR",8,125,mouse[0],mouse[1],click[0],buttonfont)
		button(5,160,40,40,255,153,51,255,204,153,"multi","MULTI",10,175,mouse[0],mouse[1],click[0],buttonfont)
		button(55,10,40,40,51,153,255,153,204,255,"ir","IR",70,25,mouse[0],mouse[1],click[0],buttonfont)
		button(55,60,40,40,255,255,0,255,255,204,"us","US",69,75,mouse[0],mouse[1],click[0],buttonfont)
	else:
		button(5,150,40,40,178,102,255,204,153,255,"tape","TAPE",10,165,mouse[0],mouse[1],click[0],buttonfont)
		button(5,100,40,40,255,255,0,255,255,204,"us","US",19,115,mouse[0],mouse[1],click[0],buttonfont)
		button(5,200,40,40,0,255,128,153,255,204,"overhead","OVER",11,215,mouse[0],mouse[1],click[0],buttonfont)
		button(5,250,40,40,255,102,102,255,153,153,"radar","RADAR",8,265,mouse[0],mouse[1],click[0],buttonfont)
		button(5,50,40,40,51,153,255,153,204,255,"ir","IR",20,65,mouse[0],mouse[1],click[0],buttonfont)
		button(5,300,40,40,255,153,51,255,204,153,"multi","MULTI",10,315,mouse[0],mouse[1],click[0],buttonfont)

def plot_point(xcenter,ycenter): #draws cross point
	pygame.draw.line(screen, (255,0,0),(xcenter - 2.5,ycenter - 2.5),(xcenter + 2.5,ycenter + 2.5),2)
	pygame.draw.line(screen, (255,0,0),(xcenter + 2.5,ycenter -2.5),(xcenter - 2.5,ycenter + 2.5),2)

def draw_lines(lines): #draw overheadlines
	pygame.draw.lines(screen, (255,51,51), False, lines,2)

##############################################
#                SHOW RAW DATA               #
##############################################

def display_rawir():
	global datafont, ir_raw, ir
	raw_ir = "RAW IR: " + str(ir_raw)
	irdis = "IR: " + str(ir)
	data = font.render(raw_ir, True, (255,255,255))
	screen.blit(data, (250,3))
	data = font.render(irdis, True, (255,255,255))
	screen.blit(data, (450,3))

def display_rawus():	
	global datafont, us_raw, us
	raw_us = "RAW US: " + str(us_raw)
	usdis = "US: " + str(us)
	data = font.render(raw_us, True, (255,255,255))
	screen.blit(data, (250,3))
	data = font.render(usdis, True, (255,255,255))
	screen.blit(data, (450,3))

def endofsweep():
	global sweep_num, lastsweep, pointlist, rpointlist, a,  m, times
	if (lastsweep != sweep_num):
		a = 0
		pointlist = []
		times = no_points
		rpointlist = []
		lastsweep = sweep_num
	


##############################################
#                  GLOBALS                   #
##############################################
clock = pygame.time.Clock()
count = 0
currentline = 0
tick = 20
no_points = 35
pygame.init() #essential 
colour = (255,255,255) #background colour
buttonfont = pygame.font.SysFont("Arial", 10) #buttons font
font = pygame.font.SysFont("Arial", 25) #number font 
datafont = pygame.font.SysFont("Arial", 7)
sensor = "ir" #initial sensor
mode = "tape" #initial mode 
ir_raw = 0 #current raw ir
us_raw = 0 #current raw us
ir = 0 #current ir
us = 0 #current us
angle = 0 #current angle
max_rot = 0 #max rotation - multiview and radar mode
min_rot = 0 #min rotation - multiview and radar mode
cal_point = 0 #for cal mode a single calibration point
sweep_num = 0 #sweep its on for multiview mode
a = 0 #overhead x val point
line = 0
p0 = [] #for multiview mode
p1 = []
p2 = []
p3 = []
oha = 0
rpointlist = []
mcount = 0
previous = "tape"
lines = []   #line in saved file
pointlist = [] #points for line to draw between
screen=pygame.display.set_mode((800,400)) #set and lock display size do not change as math is dependant on size
pygame.mouse.set_visible(1) #enables use of mouse
calpoints = []
previousmode = "ir"
lastdist = 0
lastsweep = 0
ccount = 0
minimum11 = 1000
minimum12 = 1000
minimum21 = 1000
minimum22 = 1000
minimum31 = 1000
minimum32 = 1000
minimum41 = 1000
minimum42 = 1000
flipped1 = []
flipped2 = []
flipped3 = []
flipped4 = []
checker = 0
distance = 0
join1 = 135
join2 = 225
join3 = 315
join4 = 45
status = False
maximum1 = [0, 0]
maximum2 = [0, 0]
maximum3 = [0, 0]
maximum4 = [0, 0]
modecheck = 0
quad1 = []
quad2 = []
quad3 = []
quad4 = []
scan1 = []
scan2 = []
scan3 = []
scan4 = []
c = 0
newmulti = 0
limit = 80

def readfile(): #need to read from certain line
	global currentline, ir,us, ir_raw, us_raw, angle, max_rot, min_rot, cal_point, sweep_num
	fp = open("data.tmp", "r")
	for i, line in enumerate(fp):
		if i == (currentline):
			x = line
			x = x.split(':',10)
			fileline = x[0]
			ir_raw = x[1]
			us_raw = x[2]
			ir = int(x[3])
			us = int(x[4])
			angle = int(x[5])
			max_rot = int(x[6])
			min_rot = int(x[7])
			cal_point = int(x[8])
			sweep_num = int(x[9])
		else:
			pass	
	fp.close()
	currentline += 1	


main()
