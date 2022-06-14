#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "serial.h"
#include "serial.c"


#define adcfunc 1
#define adcport 0
#define adcpin1 23 // mbed out pin is 15
#define adcpin2 24 // mbed out pin is 16

void adc_init(void);

int adc_is_busy(int channel);

void adc_init(void){
    //initialises ADC with regards to the pins setup via pinsettings
    PINSEL_CFG_Type PinCfg;
    pin_settings(PinCfg, adcfunc, 0, 0, adcport, adcpin1);
    //pin_settings(PinCfg, adcfunc, 0, 0, adcport, adcpin2);
    ADC_Init((LPC_ADC_TypeDef *)LPC_ADC, 20);
    ADC_ChannelCmd((LPC_ADC_TypeDef *)LPC_ADC, 0, ENABLE);
    //ADC_ChannelCmd((LPC_ADC_TypeDef *)LPC_ADC, 1, ENABLE);
    ADC_IntConfig((LPC_ADC_TypeDef *) LPC_ADC, ADC_ADINTEN0, SET);
    //ADC_IntConfig((LPC_ADC_TypeDef *) LPC_ADC, ADC_ADINTEN1, SET);
}

int adc_is_busy(int channel){
    return ADC_ChannelGetStatus((LPC_ADC_TypeDef *)LPC_ADC, channel, 0);
}

int adc_get_data(int channel){
    return ADC_ChannelGetData((LPC_ADC_TypeDef *)LPC_ADC, channel);
}

uint16_t get_data_and_print(void){
    //if the adc is not busy then write to the terminal teh values obtained from the adc.
    while (adc_is_busy(0)){
        continue;
    }
    uint16_t x;
    x = adc_get_data(0);
    //char port[6] = "";
    //sprintf(port, "%i", x);
    //write_usb_serial_blocking(port, 6);
    //write_usb_serial_blocking("\n\r", 2);
    return x;
}

uint16_t get_data(void){
    //if the adc is not busy then write to the terminal teh values obtained from the adc.
    while (adc_is_busy(0)){
        continue;
    }
    uint16_t x;
    x = adc_get_data(0);
    return x;
}

void ADC_IRQHandler(void){
    //(???)
    uint16_t x = adc_get_data(0);
    char port[4] = "";
    sprintf(port, "%i", x);
    write_usb_serial_blocking(port, 4);
    write_usb_serial_blocking("\n\r", 2);
}//
