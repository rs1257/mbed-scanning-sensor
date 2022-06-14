void keypad_init(int i2c_port);

void write_keyboard_pin(uint8_t pin, int i2c_port);

unsigned char read_keypad_main(int i2c_port);

char read_keypad(int i2c_port);

char determine_key_pressed(unsigned char retint);

char keypad_check(char x, char prev);