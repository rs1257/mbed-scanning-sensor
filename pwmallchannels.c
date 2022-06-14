//pwm port is 2
//pwm pins are 0-2, 3-5, mbed pins 28, 25, 24, 23, 22, 21
#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"


#define pwmport 2
#define pwmpin 1
#define pwmfunc 1


void pwm_update_high (void);
void pwm_update_low (void); 

void pwm_init(void){
    PINSEL_CFG_Type PinCfg;
    PWM_TIMERCFG_Type PWMCfg;
    PWM_MATCHCFG_Type PWMMatchCfg;

    PWMCfg.PrescaleOption = PWM_TIMER_PRESCALE_USVAL;
    PWMCfg.PrescaleValue = 85;

    PWM_Init((LPC_PWM_TypeDef *) LPC_PWM1, PWM_MODE_TIMER, &PWMCfg);
	
	for (channelno = 0; channelno <= 5; channelno++){
		pin_settings(PinCfg, pwmfunc, 0, 0, pwmport, channelno);
	}

    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,0,256,PWM_MATCH_UPDATE_NOW);

    PWMMatchCfg.IntOnMatch = DISABLE;
    PWMMatchCfg.MatchChannel = 0;
    PWMMatchCfg.ResetOnMatch = ENABLE;
    PWMMatchCfg.StopOnMatch = DISABLE;
    PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
	
	for (channelno = 2; channelno < 7; channelno++){
		PWM_ChannelConfig((LPC_PWM_TypeDef *) LPC_PWM1, channelno, PWM_CHANNEL_SINGLE_EDGE);
	}
	
	for (channelno = 1; channelno < 7; channelno++){
		PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,channelno,0,PWM_MATCH_UPDATE_NOW);
		PWMMatchCfg.IntOnMatch = DISABLE;
		PWMMatchCfg.MatchChannel = channelno;
		PWMMatchCfg.ResetOnMatch = DISABLE;
		PWMMatchCfg.StopOnMatch = DISABLE;
		PWM_ConfigMatch((LPC_PWM_TypeDef *) LPC_PWM1, &PWMMatchCfg);
		PWM_ChannelCmd((LPC_PWM_TypeDef *) LPC_PWM1, channelno, ENABLE);
	}
    
	PWM_ResetCounter((LPC_PWM_TypeDef *)LPC_PWM1);
    PWM_CounterCmd((LPC_PWM_TypeDef *)LPC_PWM1, ENABLE);
    PWM_Cmd((LPC_PWM_TypeDef *) LPC_PWM1, ENABLE);    
}


void pwm_update_high (void){
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,0,PWM_MATCH_UPDATE_NOW);
    
}

void pwm_update_low (void){
    PWM_MatchUpdate((LPC_PWM_TypeDef *) LPC_PWM1,2,256,PWM_MATCH_UPDATE_NOW);
}
