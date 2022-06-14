//LCD Port is 59
//Character set is R
#include "lpc17xx_i2c.h"
#include "lcd_display.h"
#include "lpc_types.h"
#include "TransferCfg.h"
#include "TransferCfg.c" //Includes: "types.h"

#define emptychar 0xA0
#define usedi2c LPC_I2C1

void write_display(int i2c_port, uint8_t address, char char_to_send);
uint8_t alloc_lcd_addr(uint8_t addr, int i, char* str_to_write);
//(???)
uint8_t alloc_lcd_addr(uint8_t addr, int i, char* str_to_write){
    if (addr == (0x80 + 16)){
        addr = 0x80 + 40;
        write_display(59, addr, (str_to_write[i]) | 0x80);
        addr++;
        return addr;
    }
    else{
        write_display(59, addr, (str_to_write[i]) | 0x80);
        addr++;
        return addr;
    }
}

void display_init(int i2c_port){
    /*while (read_busy_flag_display(i2c_port) == 1){
        // empty while just to check the flag
    }*/
    //(???)
    sleep(10000);
    uint8_t write[11] = {0x00,0x34,0x0c,0x06,0x35,0x04,0x10,0x42,0x9f,0x34,0x02};
    I2C_M_SETUP_Type TransferCfg;
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, write, 11, NULL, 0);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    sleep(1000);
    uint8_t write2[2] = {0x00, 0x01};
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, write2, 2, NULL, 0);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    sleep(2000);
    clear_display(i2c_port);
}

void clear_display(int i2c_port){
    //Replaces all values on the lcd display with " " and resets the typing location back to start.
    uint8_t addr = 0x80;
    int i;
    for (i=0; i <33; i++){
        if (addr == (0x80 + 16)){
            addr = 0x80 + 40;
            write_display(i2c_port, addr, emptychar);
            addr++;
        }
        else{
            write_display(i2c_port, addr, emptychar);
            addr++;
        }
    }
}

void write_display(int i2c_port, uint8_t address, char char_to_send){
    /*while (read_busy_flag_display(i2c_port) == 1){
        write_usb_serial_blocking("busy1\n\r", 7);
    }*/
    //Writes a char to the lcd then starts a sleep loop in order to make a delay between key presses.
    uint8_t writeadd[2] = {0x00, address};
    uint8_t writedata[2] = {0x40, char_to_send};
    sleep(1000);

    I2C_M_SETUP_Type TransferCfg;
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, writeadd, 2, NULL, 0);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);

    /*while (read_busy_flag_display(i2c_port) == 1){
        write_usb_serial_blocking("busy2\n\r", 7);
    }*/
    sleep(1000);
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, writedata, 2, NULL, 0);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
}

int read_busy_flag_display(int i2c_port){
    //(???)
    uint8_t write[1];
    write[1] = 0x80;
    int receive[1];
    I2C_M_SETUP_Type TransferCfg;
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, write, 1, receive, 1);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    receive[1] &= 0x80;
    if (receive[1] == 0x80){
        return 1; // busy is true
    }
    else{
        return 0;
    }
}

void sleep(int loopcycles){
    int i;
    //delay for amount of time determined by loopcycles.
    for (i = 0; i < loopcycles; i++){
    }
}
