/*
 * File:   main.c
 * Author: shu
 *
 * Created on 2017/03/15, 17:10
 */


#include <xc.h>
#include "display.h"

#pragma config FOSC = INTOSC
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF

#pragma config WRT = OFF
#pragma config PLLEN = OFF 
#pragma config STVREN = ON
#pragma config BORV = HI
#pragma config LVP = OFF

void init();
void demo();

void main(void) {
    init();
    
    while(1) {
        demo();
    }
    
    return;
}

void init() {
     OSCCON = 0b01110010;
     ANSELA = 0b00000000;
     ANSELB = 0b00000000;
     TRISA  = 0b00000000;
     TRISB  = 0b00000000;
     PORTA  = 0b00000000;
     PORTB  = 0b00000000;
}

void demo() {
    static int nums[2];
    static int dots[2] = { ON, OFF };
    int i, j;
    
    for (i = 0; i < 11; i++) {
        nums[0] = i;
        nums[1] = i;
        for (j = 0; j < 100; j++) {
            // ???????????7??????????
            // ?????????,?????????
            light2( nums, dots );
        }
    }
    
    for (i = 0; i < 100; i++) {
        //??????????
        error2();
    }
}