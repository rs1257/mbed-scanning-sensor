//keypad port is 33
//write column - upper nibble
//read row - lower nibble
#include "lpc17xx_i2c.h"
#include "lpc_types.h"
#include "keypad.h"

#define usedi2c LPC_I2C1

void keypad_init(int i2c_port){
    //initialises the keypad on port specified by i2c_port.
    int init[1] = {0xFF};
    I2C_M_SETUP_Type TransferCfg;
    TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, init, 1, NULL, 0);
    I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
}

void write_keyboard_pin(uint8_t pin, int i2c_port){
    //determines key pressed on a row determined by pin
    if (pin == 0){
        int buff[1] = {0xEF};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
    if (pin == 1){
        int buff[1] = {0xDF};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
    if (pin == 2){
        int buff[1] = {0xBF};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
    if (pin == 3){
        int buff[1] = {0x7F};
        I2C_M_SETUP_Type TransferCfg;
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, buff, 1, NULL, 0);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
    }
}

unsigned char read_keypad_main(int i2c_port){
    //Main function that reads from the keypad by iterating through  and writing to pins on the keypad
    // and then reading from them to see if a key is pressed.
    uint8_t i;
    for (i = 0; i < 4; i++){
        keypad_init(i2c_port);
        write_keyboard_pin(i, i2c_port);
        unsigned char sendbuff[1] = {0xDF};
        unsigned char receivebuff[1];
        I2C_M_SETUP_Type TransferCfg;
        //TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, sendbuff, 1, NULL, 0);
        //I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
        int j;
        for (j = 0; j <100000; j++){
            int x = 0;
        }
        TransferCfg = setup_TransferCfg(TransferCfg, i2c_port, NULL, 0, receivebuff, 1);
        I2C_MasterTransferData(usedi2c, &TransferCfg, I2C_TRANSFER_POLLING);
        char out;
        out = determine_key_pressed(TransferCfg.rx_data[0]);
        if (out != 'Z') {
            return out;
        }
    }
    return 'Z';
}

char read_keypad(int i2c_port){
    unsigned char retint;
    return read_keypad_main(i2c_port);
    //return determine_key_pressed(retint);
}

char determine_key_pressed(unsigned char retint){
    //lookup values for each potential value given by read keypad.
    switch (retint){
        case 0x77: return '1';
        case 0xB7: return '2';
        case 0xD7: return '3';
        case 0xE7: return 'A';
        case 0x7B: return '4';
        case 0xBB: return '5';
        case 0xDB: return '6';
        case 0xEB: return 'B';
        case 0x7D: return '7';
        case 0xBD: return '8';
        case 0xDD: return '9';
        case 0xED: return 'C';
        case 0x7E: return '*';
        case 0xBE: return '0';
        case 0xDE: return '#';
        case 0xEE: return 'D';
        default: return 'Z';
    }
}

char keypad_check(char x, char prev){
    //checks if the last key pressed is identical to the current keypress.
    //If it is not, prev is updated with the new key.
    //If no key is pressed then prev is set to Z.
    if (x == prev){
        return prev = x;
    }
    else if (x != 'Z' && x != prev){
        prev = x;
        return prev;
    }
    else if (x == 'Z' && prev != 'Z'){
        return prev = 'Z';
    }
    else{
        return prev == "Z";
    }
}

void keypad_change_servo_speed(int* turn_speed, char input_key, char* previous_key){
    //switch to change the servo speed on press of * key.
    //servo will increase in speed on each press until the loop
    //the values are how often the servo PWM is updated, lower value, more often it is updated
    if (input_key == '*' && *previous_key != input_key){
        *previous_key = '*';
        switch(*turn_speed){
            case 50: *turn_speed = 25;return;
            case 25: *turn_speed = 15;return;
            case 15: *turn_speed = 8;return;
            case 8: *turn_speed = 2;return;
            case 2: *turn_speed = 50;return;
        }
    }
    else return;
}

void keypad_change_servo_start_pos(int* min_pos_num, char input_key, char* previous_key){
    //changes minimum count value to change the minimum possible angle.
    if (input_key == '0' && *previous_key != input_key){
        *previous_key = '0';
        switch(*min_pos_num){
            case 8: *min_pos_num = 11;return;
            case 11: *min_pos_num = 15;return;
            case 15: *min_pos_num = 19;return;
            case 19: *min_pos_num = 8;return;
        }
    }
    else return;
}

void keypad_change_servo_stop_pos(int* max_pos_num, char input_key, char* previous_key){
    //changes maximum count value to change the maximum possible angle.
    if (input_key == '#' && *previous_key != input_key){
        *previous_key = '#';
        switch(*max_pos_num){
            case 28: *max_pos_num = 25;return;
            case 25: *max_pos_num = 21;return;
            case 21: *max_pos_num = 17;return;
            case 17: *max_pos_num = 28;return;
        }
    }
    else return;
}

void keypad_change_sample_rate(int* sample_rate, char input_key, char* previous_key){
    //changes sample rate of sensors. These values are how often the timer that controls them fires its interrupt
    //so the higher the value, the less often it fires.
    if (input_key == '8' && *previous_key != input_key){
        *previous_key = '8';
        switch(*sample_rate){
            case 2500: *sample_rate = 5000;return;
            case 5000: *sample_rate = 10000;return;
            case 10000: *sample_rate = 15000;return;
            case 15000: *sample_rate = 20000;return;
            case 20000: *sample_rate = 40000;return;
            case 40000: *sample_rate = 2500;return;
        }
    }
    else return;
}

void keypad_activate_interrupt(char input_key, char* previous_key){
    //changes maximum count value to change the maximum possible angle.
    if (input_key == '3' && *previous_key != input_key){
        *previous_key = '3';
        SYSTICK_IntCmd(ENABLE);
        newmulti = 1;
    }
    else {
        newmulti = 0;
        return;}
}