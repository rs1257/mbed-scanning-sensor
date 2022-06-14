#include "lpc17xx_pinsel.h"
#include "lpc_types.h"

void pin_settings (PINSEL_CFG_Type PinCfg, int funcnumber, int drain, int mode,
    int port, int pin){
    PinCfg.Funcnum = funcnumber;
    PinCfg.OpenDrain = drain;
    PinCfg.Pinmode = mode;
    PinCfg.Portnum = port;
    PinCfg.Pinnum = pin;
    PINSEL_ConfigPin(&PinCfg);
}