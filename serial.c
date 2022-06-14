#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h" 
#include "lpc_types.h"
#include "pinsettings.h"
#include "pinsettings.c"

int read_usb_serial_none_blocking(char *buf,int length)
{
    return(UART_Receive((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)buf, length, NONE_BLOCKING));
}

// Write options
int write_usb_serial_blocking(char *buf,int length)
{
    return(UART_Send((LPC_UART_TypeDef *)LPC_UART0,(uint8_t *)buf,length, BLOCKING));
}

// init code for the USB serial line
void serial_init(void){
    UART_CFG_Type UARTConfigStruct;         // UART Configuration structure variable
    UART_FIFO_CFG_Type UARTFIFOConfigStruct;    // UART FIFO configuration Struct variable
    PINSEL_CFG_Type PinCfg;             // Pin configuration for UART

    pin_settings(PinCfg, 1, 0, 0, 0, 2);
    pin_settings(PinCfg, 1, 0, 0, 0, 3);

    UART_ConfigStructInit(&UARTConfigStruct);
    UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);
    UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);
    UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);
    UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
    
}