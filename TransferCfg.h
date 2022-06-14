I2C_M_SETUP_Type setup_TransferCfg(I2C_M_SETUP_Type TransferCfg, int addr, unsigned char *wrtptr, int wrtlength,
    unsigned char *rdptr, int rdlength);

I2C_M_SETUP_Type change_write_data(I2C_M_SETUP_Type TransferCfg, 
    unsigned char *ptr, int data);