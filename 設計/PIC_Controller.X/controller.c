/*
 * File:   controller.c
 * Author: isaka-PC
 *
 * Created on 2017/04/17
 */

#include "adcon.h"
#include "controller.h"
#include "config.h"

int controller_current_limit = 0;

/* analog pin measurement */
void measure(int *value, int pin_select) {
    *value = adconv(pin_select);
}

/* PWM width calculation */
void calc(int ref, int mes,int *output){
    int res;

    *output = 0;
    res = ref - mes;
    *output += P_calc(res);
    *output += I_calc(res);
//    *output += D_calc(res);
    *output = CLIP(*output, 1023, 0);
}


int P_calc(int res) {
    int temp;

    temp = (int) (res * KP);
    return temp;
}

int I_calc(int res) {
    int temp;
    static int sum = 0;

    sum += res;
    sum = CLIP(sum, 1023, 0);
    temp = (int) (sum * KI);
    return temp;
}

int D_calc(int res) {
    int temp;
    static int previous = 0;

    temp = (int) ((res - previous) * KD);
    previous = res;
    return temp;
}

/* PWM width output */
void output(int out) {
    CCPR3L = out >> 2;
    CCP3CONbits.DC3B = out & 0x03;
}

/* timer interrupt */
void interrupt_controller(void) {
    if(TMR2IF == 1) {
        int current;
        int volume;
        int out;
        measure(&current, ADCON_RA1);
        measure(&volume, ADCON_RA0);
        current = current_conversion(current);
        set_current_limit(volume);
        calc(current,volume,&out);
        output(out);
        TMR2IF = 0;
    }
}

void init_PWM(void) {
    CCPTMRS = 0b00000000;
    CCP3CON = 0b00001100;
    T2CON = 0b00000000;
    TMR2IE = 1;
    PR2 = 255;
    CCPR3L = 0;
    CCPR3H = 0;
    TMR2 = 0;
    TMR2ON = 1;
}

/**
 * current conversion
 * @param measured_value
 * @return
 */
int current_conversion(int measured_value) {
    /*
     * WRITE CONVERSION CODE
     */
    int current = (((long) (measured_value - 0x0200)) * ((long) 1000) * ((long) 20)) >> 10;
    return AMPARE_TO_BINARY(current);
}

void set_current_limit(int current_limit) {
    controller_current_limit = current_limit;
}

int get_current_limit(void) {
    return controller_current_limit;
}