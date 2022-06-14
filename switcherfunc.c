//////////////////////////////////////////////////////////////
//                  IR/US SENSOR SWITCHER                   //
//////////////////////////////////////////////////////////////

/*
Both these functions change the sensor values displayed on the lcd screen using the keypad.
Fairly self explanatory, very similar to the keypad_change functions in keypad.c
*/

void sensor_changer(int* selector_value, char* previous_key){
    char b = read_keypad(33);
    if (b == '1' && *previous_key != b){
        clear_display(59);
        //char port[1];
        //sprintf(port, "%i", *selector_value);
        //write_usb_serial_blocking(port, 1);
        *previous_key = b;
        if (*selector_value == 0){
                *selector_value = 1;
                return;
        }
        else {
                *selector_value = 0;
                return;
        }        
    }         
}

void sensor_changer_cali_mode(int* selector_value, char* previous_key){
    char b = read_keypad(33);
    if (b == '*' && *previous_key != b){
        clear_display(59);
        char port[1];
        sprintf(port, "%i", *selector_value);
        //write_usb_serial_blocking(port, 1);
        *previous_key = b;
        if (*selector_value == 0){
                *selector_value = 1;
                return;
        }
        else {
                *selector_value = 0;
                return;
        }        
    }         
}