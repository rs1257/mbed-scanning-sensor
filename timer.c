#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"

#define pin 0x4

void pinsetup(void);
void timer_init(void);

void ultrasound(void){
  pinsetup();
  timer_init();
  GPIO_SetDir(2, pin, 1);
}

void timer_init(void){

  TIM_TIMERCFG_Type TIMERCfg;
  TIM_CAPTURECFG_Type RISINGCfg;
  TIM_CAPTURECFG_Type FALLINGCfg;
  TIM_MATCHCFG_Type PULSECfg;

  TIMERCfg.PrescaleOption = TIM_PRESCALE_USVAL;
  TIMERCfg.PrescaleValue  = 100;
  TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIMERCfg);
  TIM_Init(LPC_TIM2, TIM_TIMER_MODE, &TIMERCfg);
  TIM_Init(LPC_TIM3, TIM_TIMER_MODE, &TIMERCfg);

  RISINGCfg.CaptureChannel = 0;
  RISINGCfg.FallingEdge = DISABLE;
  RISINGCfg.IntOnCaption = ENABLE;
  RISINGCfg.RisingEdge = ENABLE;
  TIM_ConfigCapture(LPC_TIM2, &RISINGCfg);

  FALLINGCfg.CaptureChannel = 1;
  FALLINGCfg.FallingEdge = ENABLE;
  FALLINGCfg.IntOnCaption = ENABLE;
  FALLINGCfg.RisingEdge = DISABLE;
  TIM_ConfigCapture(LPC_TIM3, &FALLINGCfg);

  PULSECfg.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
  PULSECfg.IntOnMatch = ENABLE;
  PULSECfg.MatchChannel = 0;
  PULSECfg.ResetOnMatch = DISABLE;
  PULSECfg.StopOnMatch = DISABLE;
  TIM_ConfigMatch(LPC_TIM0, &PULSECfg);

  TIM_UpdateMatchValue(LPC_TIM0, 0, 200);

  TIM_Cmd(LPC_TIM0, ENABLE);
  TIM_Cmd(LPC_TIM2, ENABLE);
  TIM_Cmd(LPC_TIM3, ENABLE);

  NVIC_EnableIRQ(TIMER0_IRQn);
  NVIC_EnableIRQ(TIMER2_IRQn);
  NVIC_EnableIRQ(TIMER3_IRQn);  
  }

void pinsetup(void){
  PINSEL_CFG_Type PinCfg;
  pin_settings(PinCfg, 3, 0, 0, 0, 24);
  pin_settings(PinCfg, 3, 0, 0, 0, 4);
  }
  
