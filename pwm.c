//pwm port is 2
//pwm pins are 0-2, 3-5, mbed pins 28, 25, 24, 23, 22, 21
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"


#define pwmport 2
#define pwmpin 1
#define pwmfunc 1
#define channel 2

void pwm_init(void){
    PINSEL_CFG_Type PinCfg;
    PWM_TIMERCFG_Type PWMCfg;

    PWM_MATCHCFG_Type PWMMatchCfg;
    //defines different type variables
    PWMCfg.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
    PWMCfg.PrescaleValue = 85;
    //Determines the length of gap between pulses.
    PWM_Init((LPC_PWM_TypeDef *) LPC_PWM1, PWM_MODE_TIMER, &PWMCfg);
    //initialises pwm in timer mode with regard to PWMcfg, creating a type "LPC_PWM1".
    pin_settings(PinCfg, pwmfunc, 0, 0, pwmport, pwmpin);
    //sets up the pins that are used by the PWM
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,0,256,PWM_MATCH_UPDATE_NOW);
    //Sets the pwm as max value in order to initialise it.

    PWMMatchCfg.IntOnMatch = DISABLE;
    PWMMatchCfg.MatchChannel = 0;
    PWMMatchCfg.ResetOnMatch = ENABLE;
    PWMMatchCfg.StopOnMatch = DISABLE;
    //Sets up the config settings for the pwm
    PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
    

    PWM_ChannelConfig((LPC_PWM_TypeDef *) LPC_PWM1, channel, PWM_CHANNEL_SINGLE_EDGE);
    //Sets channel 1 and channel 2 and singel edge mode pwms.
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,channel,0,PWM_MATCH_UPDATE_NOW);
    //sets the values of channels 1 and 2 as 0 to reset them from the max value they were given earlier.
    PWMMatchCfg.IntOnMatch = DISABLE;
    PWMMatchCfg.MatchChannel = channel;
    PWMMatchCfg.ResetOnMatch = DISABLE;
    PWMMatchCfg.StopOnMatch = DISABLE;
    PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
    PWM_ChannelCmd((LPC_PWM_TypeDef *) LPC_PWM1, channel, ENABLE);
    //Reconfigures the settings for channel 1
    //Reconfigures the settings for channel 2
    PWM_ResetCounter((LPC_PWM_TypeDef *)LPC_PWM1);
    PWM_CounterCmd((LPC_PWM_TypeDef *)LPC_PWM1, ENABLE);  
    //Resets and enables pwm counter (???)
}

void pwm_enable(void){
    PWM_Cmd((LPC_PWM_TypeDef *) LPC_PWM1, ENABLE);
    //Enables the PWM
}


