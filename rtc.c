#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_rtc.h"

#define rtcport 0
#define rtcfunc 0
#define rtcpin 16
//defines values for port no. function no. and pin no.
void rtc_init(void);

//RTC_IRQHandler (void);

void rtc_init(void){
    //initialises the real time clock with regards to the values defined above.

    PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, rtcfunc, 0, 0, rtcport, rtcpin);
    RTC_Init((LPC_RTC_TypeDef *) LPC_RTC);
    RTC_ResetClockTickCounter((LPC_RTC_TypeDef *)LPC_RTC);
    RTC_CalibConfig((LPC_RTC_TypeDef *) LPC_RTC, 100, RTC_CALIB_DIR_FORWARD);
    RTC_SetTime((LPC_RTC_TypeDef *)LPC_RTC, RTC_TIMETYPE_SECOND, 0);
    RTC_CntIncrIntConfig((LPC_RTC_TypeDef *)LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
    RTC_AlarmIntConfig((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, DISABLE);
    RTC_ClearIntPending((LPC_RTC_TypeDef *)LPC_RTC, RTC_INT_ALARM);
    //RTC_SetAlarmTime((LPC_RTC_TypeDef *) LPC_RTC, RTC_TIMETYPE_SECOND, 5);
    RTC_Cmd((LPC_RTC_TypeDef *) LPC_RTC, ENABLE);
    NVIC_EnableIRQ(RTC_IRQn);
}

