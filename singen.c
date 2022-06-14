#include "math.h" 

//int sample = 100;
double f0 = 10000;
double fs = 1600000;
double angle = 0;
double twopi = 2.0 * M_PI;
double increment;
double result[512];
int i;

double dtr_or_rtd(char choice, double value);
double* sin_gen (int freq, int amp, double period, int sample);

double* sin_gen (int freq, int amp, double period, int sample){
    double offset = amp*1;
    increment = (twopi * f0)/fs;
    for (i = 0;i <= sample; i++){
            result[i] = offset + (amp * sin(freq*angle));
            angle += increment;
            if (angle >= twopi){angle -= twopi;}
        }
    return result;
}

//enter degrees or radians followed by value
double dtr_or_rtd(char choice, double value){
    choice = tolower(choice);
    if (choice == "radians"){
        return value * (180.0 / M_PI);}
    else if (choice == "degrees"){
        return value * (M_PI / 180.0);}
    else {
        return 1000.0;}
}

/*
maximum value

minimum value

time between peaks
*/