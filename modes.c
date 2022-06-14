#include "initialvalues.c"
#include "lcd_display.c"
#include "keypad.c"
#include "IR.c"
#include "rtc.c"
#include "pwm.c"
#include "systick.c"
#include <stdlib.h>
#include <string.h>
#include "timer.c"
#include "switcherfunc.c"
#include "lcddisplayfuncs.c"
#include "generalfuncs.c"
#include "interrupts.c"

int scan_mode(char previous);
int tape_measure_mode(char previous);
int calibration_mode(char previous);
void lcd_display_bottom_row();
void lcd_display_top_row(char* currentmode);
void sensor_changer(int* selector_value, char* previous_key);
void servoreset(void);
//Variables used in systick handler


//////////////////////////////////////////////////////////////
//                  MAIN MODES                              //
//////////////////////////////////////////////////////////////

/*
These are the four modes used by the system - calibration, tape measure, scan and multiview.
Calibration mode takes three reference readings from the keypad, compares them to the values it gets from the US and IR.
After getting all three values for both, it averages them and sets ir_calib_adjust and us_calib_adjust to the averaged values.
After this, calibration mode functions identically to tape measure mode.
*/

int calibration_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    count = 18;
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
    

    char a = read_keypad(33);
    /*
    Selector to change the mode using the keypad keys A,B,C and D. Replicated in the rest of the modes too.
    When the system is started for the first time, it starts in calibration mode and requests its values unless one of the
    B, C or D keys is held at startup.
    */
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        int ir_calib_arr[3];
        int us_calib_arr[3];
        int ir_reported;
        int us_reported;
        int ir_calib_val;
        int us_calib_val;
        int u = 0;
        char write[2];
        strcpy(write, "");
        /*
        Code to get values from the keypad. The user places an object at a set distance away from the head, enters how far it is from 
        the head, then finalises this value with the # key. If a mistake is made, the screen can be cleared with the A key,
        Then repostions it closer or further and enters the new distance. 
        This is repeated once more to obtain three different calibration values.
        */
        while(calib_tracker <3){
            if (u == 1000){
                sensor_changer_cali_mode(&sensor_selector, &previous);
                int addr = 0x80;
                int i;
                for(i=0; i < strlen("Input actual dist: "); i++){
                    addr = alloc_lcd_addr(addr, i, "Input actual dist: ");
                }
                for(i=0; i < strlen(write); i++){
                    addr = alloc_lcd_addr(addr, i, write);
                }
                char x;
                x = read_keypad(33);
                if (x == previous){
                    previous = x;
                }
                else if (x != 'Z' && x != previous){
                    if (x == 'A'){
                        clear_display(59);
                        strcpy(write, "");
                        previous = x;
                    }
                    else if (isalpha(x) || x == '*' || isdigit(x)){
                        addr = 0x80;
                        append(write, x);
                        clear_display(59);
                        for(i=0; i < strlen("Input actual dist: "); i++){
                            addr = alloc_lcd_addr(addr, i, "Input actual dist: ");
                        }
                        for(i=0; i < strlen(write); i++){
                            addr = alloc_lcd_addr(addr, i, write);
                        }
                        previous = x;
                    }
                    else if(x == '#'){
                        clear_display(59);
                        int act_val = atoi(write);  
			int anglemax = ((servo_stop-8)*9);
        		int anglemin = ((servo_start-8)*9);
        		int angle = ((count-8)*9);
                        char port3[90] = "";
                        sprintf(port3, ";%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;\n\r", ir_raw, us_raw, ir_dist, us_dist, angle, anglemax, anglemin, act_val, sweep_num, multicheck, mode, newmulti);
	                    write_usb_serial_blocking(port3 ,90);
                        ir_reported = ir_dist;
                        char port[3] = "";
                        sprintf(port, "%i", ir_reported);
                        //write_usb_serial_blocking("ir_report: ", 11);
                        //write_usb_serial_blocking(port, 3);
                        //write_usb_serial_blocking("\n\r", 2); 
                        us_reported = us_dist;
                        char port1[3] = "";
                        sprintf(port1, "%i", us_reported);
                        //write_usb_serial_blocking("us_report: ", 11);
                        //write_usb_serial_blocking(port1, 3);
                        //write_usb_serial_blocking("\n\r", 2); 
                        ir_calib_val = (act_val - ir_reported);
                        us_calib_val = (act_val - us_reported);
                        ir_calib_arr[calib_tracker] = ir_calib_val;
                        us_calib_arr[calib_tracker] = us_calib_val;
                        calib_tracker++;
                        strcpy(write, "");
                        previous = x;
                    }
                }
                else if (x == 'Z' && previous != 'Z'){
                    previous = 'Z';
                }
            }
            else{
                u++;
            }
        }
        /*
        If the calibration values have been entered, but the calibration is not yet complete, this calculates the calibration adjustments
        for both IR and US, then sets calibrated_flag to stop them from being recalculated.
        */
        if (calibrated_flag == 0){
            for (calib_tracker = 0; calib_tracker <3; calib_tracker++){
                ir_calib_total += ir_calib_arr[calib_tracker];
                us_calib_total += us_calib_arr[calib_tracker];
            }
            calibrated_flag = 1;
            ir_calibration_adjust = ir_calib_total/3;
            us_calibration_adjust = us_calib_total/3;
            char port1[3] = "";
            sprintf(port1, "%i", ir_calib_total);
            /*write_usb_serial_blocking("IR total: ", 8);
            write_usb_serial_blocking(port1, 3);
            write_usb_serial_blocking("\n\r", 2);*/
            char port2[3] = "";
            sprintf(port2, "%i", us_calib_total);
            /*write_usb_serial_blocking("US total: ", 8);
            write_usb_serial_blocking(port2, 3);
            write_usb_serial_blocking("\n\r", 2);*/
            char port3[3] = "";
            sprintf(port3, "%i", ir_calibration_adjust);
            /*write_usb_serial_blocking("IR adjust: ", 9);
            write_usb_serial_blocking(port3, 3);
            write_usb_serial_blocking("\n\r", 2);*/
            char port4[3] = "";
            sprintf(port4, "%i", us_calibration_adjust);
            /*write_usb_serial_blocking("US adjust: ", 9);
            write_usb_serial_blocking(port4, 3);
            write_usb_serial_blocking("\n\r", 2);*/
        }

        lcd_display_top_row("Cali");
        lcd_display_bottom_row();
        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
        //keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);
        return 0;
    }
}

/*
In tape measure mode, the sensor head points straight ahead and measures values at the sample rate, which can be changed using the 8 key.
By default these values are shown on screen as IR, but this can be changed to US using the 1 key. This also applies to the rest ofr the modes.
*/

int tape_measure_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    count = 18;
    lcd_display_top_row("Tape");
    lcd_display_bottom_row();
    

    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,18,PWM_MATCH_UPDATE_NOW);
        keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);
        return 1;
    }
}

/*
In scan mode, the sensor head rotates from 0 to 180 degrees at a speed specified by the user, using the * key. The 0 and # keys
can be used to change the position at which the servo starts and stops its rotation, respectively.
The head still takes readings at its specified sample rate.
*/

int scan_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    lcd_display_top_row("Scan");
    lcd_display_bottom_row();
    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        keypad_change_servo_speed(&turnspeed, a, &previous);
        keypad_change_servo_start_pos(&servo_start, a, &previous);
        keypad_change_servo_stop_pos(&servo_stop, a, &previous);
        keypad_change_sample_rate(&samplerate, a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);

        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 2;
    }
}

/*
Multi-view mode is very similar to Sscan mode in this C file. Most of the differences are in the accompanying python graph code.
*/

int multi_view_mode(char previous){
    sensor_changer(&sensor_selector, &previous);
    lcd_display_top_row("Mult");
    lcd_display_bottom_row();
    //use sweep_num to see what stage of 'turning' the object is at    

    char a = read_keypad(33);
    if (a == 'A'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 0;
    }
    else if (a == 'B'&& previous != a){
        SYSTICK_IntCmd(DISABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 1;
    }
    else if (a == 'C'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        clear_display(59);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 2;
    }
    else if (a == 'D'&& previous != a){
        servoreset();
        SYSTICK_IntCmd(ENABLE);
        char a = read_keypad(33);
        previous = keypad_check(a, previous);
        return 3;
    }
    else{
        keypad_change_servo_speed(&turnspeed, a, &previous);
        keypad_change_servo_start_pos(&servo_start, a, &previous);
        keypad_change_servo_stop_pos(&servo_stop, a, &previous);
        keypad_change_sample_rate(&samplerate, a, &previous);
        keypad_activate_interrupt(a, &previous);
        average_calculator(us_dist_arr, ir_dist_arr, array_counter, &us_avg, &ir_avg);
        previous = keypad_check(a, previous);
        
        //distanceircalc();
        //RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, ENABLE);
        //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 1);
        return 3;
    }
}
