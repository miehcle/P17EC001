/*
 * File:   adcon.c
 * Author: isaka-PC
 *
 * Created on 2017/04/17
 */

#include <xc.h>

#include "adcon.h"
#include "config.h"

int adconv(int pin_select){
    int temp;

    ADCON0 = pin_select;
    __delay_us(1);
    GO_nDONE = 1;
    while(GO_nDONE);
    temp = ADRESH;
    temp = (temp << 8) | ADRESL;
    return temp;
}

void init_adcon(int anselA, int anselB){
    ANSELA = anselA;
    ANSELB = anselB;
    ADCON1 = 0b10010000;
    ADCON0 = 0b00000001;
}