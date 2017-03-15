/*
 * File:   main.c
 * Author: yuki
 *
 * Created on 2017/03/09, 9:47
 */


#include <xc.h>
#include "display.h"

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = OFF, CP = OFF, CPD = OFF, BOREN = ON, CLKOUTEN = OFF, IESO = OFF, FCMEN = OFF
#pragma config WRT = OFF, PLLEN = OFF, STVREN = ON, BORV = HI, LVP = OFF

#define ADDR 0x02
#define MAX_NUM_MUL_TEN 150             //max num * 10, int num

void interrupt_I2C(void);
void interrupt interruption(void);
void init(void);
void init_Timer4(void);
void init_I2C_slave(char addr);
void loop(void);
void num_digits_conv(unsigned char data,char *digits);

unsigned char received_data = 0;
unsigned char counter = 0;


void interrupt_I2C(void) {
    char x;
    if(SSP1STATbits.R_nW == 0) {
        /* write mode */
        if(SSP1STATbits.D_nA == 0) {
            /* address received */
            x = SSP1BUF;
        } else {
            /* data received */
            received_data = SSP1BUF;
        }
    }
}

void interrupt interruption(void) {
    if(SSP1IF == 1) {
        /* I2C interrupt */
        interrupt_I2C();
        SSP1IF = 0;             //clear interrupt flag
        SSP1CON1bits.CKP = 1;   //open SCL line
        counter = 0;
        TMR4 = 0;
    } else if (TMR4IF == 1) {
        /* Timer4 interrupt */
        if (counter < 16) counter++;
        TMR4IF = 0;             //clear interrupt flag
    }
}

void main(void) {
    init();
    init_Timer4();
    init_I2C_slave(ADDR);
    
    while(1){
        loop();
    }
    return;
}

void init(void) {
    OSCCON = 0b01111010;        //Fosc = 16MHz,PLL4x disabled
    ANSELA = 0x00;              //all of A pins digital mode
    ANSELB = 0x00;              //all of B pins digital mode
    TRISA = 0x00;               //all of A pins output mode
    TRISB = 0b00010010;         //RB1(SDA1) and RB4(SCL1) input mode, others output mode
    PORTA = 0x00;               //A pins init
    PORTB = 0x00;               //B pins init
    GIE = 1;                    //all interrupt allowed
    PEIE = 1;                   //peripheral equipment interrupt allowed
}

void init_Timer4(void) {
    T4CON = 0b01111011;         //1:16 postscaler, Timer4 off, prescaler is 64
    PR4 = 243;                  //16Hz timer
    TMR4IE = 1;                 //Timer4 interrupt enabled
    TMR4 = 0;                   //Timer4 init
    TMR4ON = 1;                 //Timer4 on
}

void init_I2C_slave(char addr) {
    SSP1STAT = 0b10000000;      //I2C 100kbps mode
    SSP1CON1 = 0b00100110;      //I2C slave mode
    SSP1CON2bits.SEN = 1;       //I2C clock stretch enabled
    SSP1ADD = addr << 1;        //I2C address setting
    SSP1IE = 1;                 //I2C interrupt enabled
    BCL1IE = 1;                 //I2C bus clash interrupt enabled
    SSP1IF = 0;                 //I2C interrupt flag cleared
    BCL1IF = 0;                 //I2C bus clash interrupt flag cleared
}

void loop(void) {
    int digit[2];
    int dot[2];
    
    if (counter < 16) {
        num_digits_conv(received_data, digit, dot);
        light2(digit, dot);
    } else {
        error2();
    }
}

void num_digits_conv(unsigned char data, int digit[2], int dot[2]) {
    int num = (data * MAX_NUM_MUL_TEN) / 255;
    
    if (num >= 100) {
        num /= 10;
        dot[0] = ON;
        dot[1] = OFF;
    } else {
        dot[0] = OFF;
        dot[1] = ON;
    }
    
        digit[0] = num % 10;
        digit[1] = (num / 10) % 10;
}