#include "lpc_types.h"

I2C_M_SETUP_Type setup_TransferCfg(I2C_M_SETUP_Type TransferCfg, int addr, unsigned char *wrtptr, int wrtlength,
    unsigned char *rdptr, int rdlength){
    TransferCfg.sl_addr7bit = addr;
    TransferCfg.tx_data = wrtptr;
    TransferCfg.tx_length = wrtlength;
    TransferCfg.rx_data = rdptr;
    TransferCfg.rx_length = rdlength;
    return TransferCfg;
    //Determines settings regarding sending or recieving down i2c bus (???)
}

I2C_M_SETUP_Type change_write_data(I2C_M_SETUP_Type TransferCfg, 
    unsigned char *ptr, int data){
    ptr[1] = data;
    TransferCfg.tx_data = ptr;
    return TransferCfg;
    // (???)
}
