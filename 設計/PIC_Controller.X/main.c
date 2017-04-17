/* 
 * File:   main.c
 * Author: isaka-PC
 *
 * Created on 2017/04/17
 */

#include <stdio.h>
#include <stdlib.h>

#include "adcon.h"
#include "controller.h"
#include "config.h"
#include "I2C.h"

void init(void);
void loop(void);
void interrupt inter(void);

/*
 * 
 */
int main(int argc, char** argv) {
    init();
    
    while(1) {
        loop();
    }

    return (EXIT_SUCCESS);
}

void init(void) {
    OSCCON = 0b01111010;        //Fosc = 16MHz,PLL4x disabled.
    TRISA = 0x03;
    TRISB = 0x00;
    PORTA = 0x00;
    PORTB = 0x00;
    for(unsigned char i = 0; i < 100; i++){}
    GIE = 1;                    //all interrupt allowed
    PEIE = 1;                   //peripheral equipment interrupt allowed
    init_adcon(0x03,0x00);
    init_PWM();
}

void loop(void) {

}


void interrupt inter(void) {
    interrupt_controller();
}