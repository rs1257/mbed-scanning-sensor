#include "modes.c"
#include "timer.c"

//NOBODY TOUCH MY INCLUDES THEY ARE DELICATE//
/*#define usedi2c LPC_I2C1
#define i2cfunc 3
#define i2cport 0
#define i2cpin1 0
#define i2cpin2 1

int count = 8;
int count2 = 0;
int i;
int mode;
mode = 0;
char a;
char previous;*///
int x = 0;
int y = 0;
int num = 0;



int main(void){
    serial_init();
    //pwm_init(2);
    //adc_init();
    //distanceircalc();
    ultrasound();
    /*PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin1);
    pin_settings(PinCfg, i2cfunc, 0, 0, i2cport, i2cpin2);
    I2C_Init(usedi2c, 100000);
    I2C_Cmd(usedi2c, ENABLE);//
    display_init(59);
    keypad_init(33);
    calibration_mode(previous);*/
    while(1){ 
        //get_data_and_print();//
        //ultrasound();//
        /*a = read_keypad(33);
        previous = keypad_check(a, previous);
        switch(mode){
            case 0: mode = calibration_mode(previous); break;
            case 1: mode = tape_measure_mode(previous); break;
            case 2: mode = scan_mode(previous); break;
        }*///
    }
}

void TIMER0_IRQHandler(void){
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    if (num == 0){
		num ++;
		GPIO_SetValue(2, pin);	
		TIM_UpdateMatchValue(LPC_TIM0, 0, 10);
	}
	else {
		num--;
		GPIO_ClearValue(2, pin);
		TIM_UpdateMatchValue(LPC_TIM0, 0, 200);
	}
    TIM_ResetCounter(LPC_TIM0);
}

void TIMER2_IRQHandler(void){
    TIM_ClearIntCapturePending(LPC_TIM2, TIM_CR0_INT);
    x = TIM_GetCaptureValue(LPC_TIM2, TIM_COUNTER_INCAP0);

}
void TIMER3_IRQHandler(void){
    TIM_ClearIntCapturePending(LPC_TIM3, TIM_CR1_INT);
    y = TIM_GetCaptureValue(LPC_TIM3, TIM_COUNTER_INCAP1);
	//int length = (y-x);
    int length = ((y - x)/58);
    char port[10] = "";
    sprintf(port, "%i", length);
    write_usb_serial_blocking(port, 10);
    write_usb_serial_blocking("\n\r", 2);
    TIM_ResetCounter(LPC_TIM2);
    TIM_ResetCounter(LPC_TIM3);
}
