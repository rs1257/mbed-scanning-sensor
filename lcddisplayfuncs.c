//////////////////////////////////////////////////////////////
//                  LCD DISPLAY FUNCTIONS                   //
//////////////////////////////////////////////////////////////

void lcd_display_top_row(char* currentmode){
    char reqspeedtodisplay[3];
    char samplespersweeptodisplay[5];
    int samples_per_s = (int)1/((float)samplerate/1000000);

    sprintf(reqspeedtodisplay,"%i",turnspeed);
    sprintf(samplespersweeptodisplay,"%i",samples_per_s);

    int addr = 0x80;
    int i;
    for (i = 0; i < strlen(currentmode); i++){
        addr = alloc_lcd_addr(addr, i, currentmode);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(reqspeedtodisplay); i++){
        addr = alloc_lcd_addr(addr, i, reqspeedtodisplay);
    }
    addr = alloc_lcd_addr(addr, 0, " ");
    for (i = 0; i < strlen(samplespersweeptodisplay); i++){
        addr = alloc_lcd_addr(addr, i, samplespersweeptodisplay);
    }
    addr = alloc_lcd_addr(addr, 0, "/");
    addr = alloc_lcd_addr(addr, 0, "s");
}


void lcd_display_bottom_row(){
    char rawvaluetodisplay[4];
    char servoangletodisplay[3];
    char distancetodisplay[3];
    char avgdistancetodisplay[3];
    int rawvalue;
    int s = 7;
    //Calculating angle of servo
    servoangle = ((count-8) * 9);
    
    //gets data from adc or ultrasound depending on mode.
    //if (sensorselector == 0){}
    //}
    //else{
        //Ultrasound data
        //rawvalue = 0;
    //}
    //Writes <6 if distance too small

    if (sensor_selector == 0){

        rawvalue = ir_raw;

        int distance = ir_dist;
        if (distance == -1){ 
            sprintf(distancetodisplay, "<%i", s);  
        }
        else{  
            sprintf(distancetodisplay, "%i", distance);
        }              
        sprintf(rawvaluetodisplay,"%i",rawvalue);
        sprintf(servoangletodisplay,"%i",servoangle);
        sprintf(avgdistancetodisplay,"%i",ir_avg);

        int addr = 0x80+16;
        int i;
    //Displays all 4 things on the bottom row in correct order
        for (i = 0; i < strlen(rawvaluetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, rawvaluetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(servoangletodisplay); i++){
            addr = alloc_lcd_addr(addr, i, servoangletodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(distancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, distancetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(avgdistancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, avgdistancetodisplay);
        }
    }

    else{

        sprintf(distancetodisplay, "%i", us_dist);
        sprintf(rawvaluetodisplay,"%i",us_raw);
        sprintf(servoangletodisplay,"%i",servoangle);
        sprintf(avgdistancetodisplay,"%i",us_avg);

        int addr = 0x80+16;
        int i;
    //Displays all 4 things on the bottom row in correct order
        for (i = 0; i < strlen(rawvaluetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, rawvaluetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(servoangletodisplay); i++){
            addr = alloc_lcd_addr(addr, i, servoangletodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(distancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, distancetodisplay);
        }
        addr = alloc_lcd_addr(addr, 0, "/");
        for (i = 0; i < strlen(avgdistancetodisplay); i++){
            addr = alloc_lcd_addr(addr, i, avgdistancetodisplay);
        }
    }

    //int distance = distanceircalc();    
    
}