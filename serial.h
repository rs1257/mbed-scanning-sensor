void serial_init();

int read_usb_serial_none_blocking(char *buf,int length);

int write_usb_serial_blocking(char *buf,int length);