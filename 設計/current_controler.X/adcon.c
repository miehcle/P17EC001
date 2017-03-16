/*
 * File:   adcon.c
 * Author: yuki
 *
 * Created on 2017/03/16, 15:29
 */


#include <xc.h>

/* to use __delay_us(), Ajust this number to your xtal frequency. */
#define _XTAL_FREQ 16000000           

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
