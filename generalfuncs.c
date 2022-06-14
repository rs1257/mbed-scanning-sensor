//////////////////////////////////////////////////////////////
//                  GENERAL FUNCTIONS                       //
//////////////////////////////////////////////////////////////

void average_calculator(int* us_arr, int* ir_arr, int counter, int* us_avg, int* ir_avg){
    //calculates the average to be outputted to the screen. Also used to clear the display
    //once the array gets to 50 and 100 in order to remove stray digits on the display.
    //if the array gets longer than 100 values, the array is reinitialised and started again from 0.
    int u;
    int us_total = 0;
    int ir_total = 0;
    if (array_counter > 100){
        clear_display(59);
        array_counter = 0;
        memset(ir_dist_arr, 0, sizeof(ir_dist_arr));
        memset(us_dist_arr, 0, sizeof(us_dist_arr));
        memset(ir_raw_arr, 0, sizeof(ir_raw_arr));
        memset(us_raw_arr, 0, sizeof(us_raw_arr));
        memset(time_arr, 0, sizeof(time_arr));
        memset(angle_arr, 0, sizeof(angle_arr));
        return;
    }
    else if (array_counter ==50){
        clear_display(59);
    }
    for (u = 0; u <= counter; u++){
        us_total += us_arr[u];
        ir_total += ir_arr[u];
    }
    *ir_avg = (ir_total/counter +1);
    *us_avg = (us_total/counter +1);
    return;
}

void append(char* s, char c){
    //small function required for calibration input to form a valid string.
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

void servoreset(void){
    //When the mode is changed to scan or multiview, the servo needs to be reset to its start point.
    //This function ensures that occurs.
        sweep_num = 0;
        multicheck = 0;
        turnspeed = 50;
        turndir = 0;
        systick_count = 0;
        count = 8;
        PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,count,PWM_MATCH_UPDATE_NOW);
        return;
}