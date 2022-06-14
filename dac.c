#include "lpc17xx_dac.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "singen.c"

#define dacport 0
#define dacfunc 2
#define dacpin 26 // mbed out pin is 18

void dac_init(void);

void dac_out(uint32_t outval);

void dac_init(void){
    PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, dacfunc, 0, 0, dacport, dacpin);
    DAC_Init((LPC_DAC_TypeDef *)LPC_DAC);
}

void dac_out(uint32_t outval){
    DAC_UpdateValue((LPC_DAC_TypeDef *)LPC_DAC, outval);
}