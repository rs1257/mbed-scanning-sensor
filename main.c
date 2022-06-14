#include "modes.c"

#define usedi2c LPC_I2C1
#define i2cfunc 3
#define i2cport 0
#define i2cpin1 0
#define i2cpin2 1
//
int count2 = 0;
//int mode = 0;
char a;
char previous;

int main(void){
    //Initialisation of all hardware in the system.
    serial_init();  
    rtc_init();
    pwm_init();
    adc_init();
    systick_init();
    ultrasound();

    PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin1);
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin2);
    I2C_Init(usedi2c, 100000);
    I2C_Cmd(usedi2c, ENABLE);
    display_init(59);
    keypad_init(33);
    SYSTICK_IntCmd(DISABLE);
    calibration_mode(previous);
    //main loop//
    while(1){
        int anglemax = ((servo_stop-8)*9);
        int anglemin = ((servo_start-8)*9);
        int angle = ((count-8)*9);
        char port[90] = "";
        ////output for graph
        sprintf(port, ";%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;%i;\n\r", ir_raw, us_raw, ir_dist, us_dist, angle, anglemax, anglemin, act_val, sweep_num, multicheck, mode, newmulti);
        write_usb_serial_blocking(port ,90);
        a = read_keypad(33);
        switch(mode){
            case 0: mode = calibration_mode(previous); break;
            case 1: mode = tape_measure_mode(previous); break;
            case 2: mode = scan_mode(previous); break;
            case 3: mode = multi_view_mode(previous); break;
        }
        //update arrays for average coalculation
        ir_dist_arr[array_counter] = ir_dist;
        ir_raw_arr[array_counter] = ir_raw;
        //sanity check for US before array assignment
        if (us_dist > 0 && us_dist < 500){
                us_dist_arr[array_counter] = us_dist;
        }
        us_raw_arr[array_counter] = us_raw;
    }
}
