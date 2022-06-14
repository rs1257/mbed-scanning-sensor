#include "adc.c"

int distanceircalc(void);

char s[3] = "< 7";

int distanceircalc(void){
    int x; 
    x = get_data();
    if (x <= 700 && x > 0){
            x = (((5461/(get_data()-17))-2)*10);
        //x = get_data_and_print();
        }
    else if (2550<= x && x <= 2630){
            x = 8;
        }
    else if (2350<= x && x <= 2550){
            x = 9;
        }
    else if (2250<= x && x <= 2350){
            x = 10;
        }
    else if (2000<= x && x <= 2250){
            x = 11;
        }
    else if (1900<= x && x <= 2000){
            x = 12;
        }
    else if (1850<= x && x <= 1900){
            x = 13;
        }
    else if (1800<= x && x <= 1850){
            x = 14;
        }
    else if (1750<= x && x <= 1800){
            x = 15;
        }
    else if (1700<= x && x <= 1750){
            x = 16;
        }
    else if (1650<= x && x <= 1700){
            x = 17;
        }
    else if (1600<= x && x <= 1650){
            x = 18;
        }
    else if (1550<= x && x <= 1600){
            x = 19;
        }
    else if (1200<= x && x <= 1550){
            x = 20;
        }
    else if (1000<= x && x <= 1200){
            x = 25;
        }
    else if (950<= x && x <= 1000){
            x = 30;
        }
    else if (850<= x && x <= 950){
            x = 35;
        }
    else if (800<= x && x <= 850){
            x = 40;
        }
    else if (750<= x && x <= 800){
            x = 45;
        }
    else if (700<= x && x <= 750){
            x = 50;
        }



    //x = get_data_and_print();


    if (x <= 2630 && x > 0){
        char port[6] = "";
            sprintf(port, "%i", x);
            //write_usb_serial_blocking(port, 6);
            //write_usb_serial_blocking("\n\r", 2);
        }
    else {
            return -1;
    }
    return x;
}
