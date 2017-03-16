/*
 * File:   main.c
 * Author: yuki
 *
 * Created on 2017/03/11, 10:30
 */


#include <xc.h>
#include "adcon.h"

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = OFF, CP = OFF, CPD = OFF, BOREN = ON, CLKOUTEN = OFF, IESO = OFF, FCMEN = OFF
#pragma config WRT = OFF, PLLEN = OFF, STVREN = ON, BORV = HI, LVP = OFF

#define ADDR 0x02

void I2C_idle_check(void);
void I2C_send_data(char addr, unsigned char data);
void interrupt interruption(void);
void init(void);
void init_Timer4(void);
void init_I2C_master(void);

unsigned char send_data = 0;

void I2C_idle_check(void) {
    while (( SSP1CON2 & 0x1F ) | (SSP1STAT & 0x05)) ;
}

void I2C_send_data(unsigned char addr, unsigned char data) {
    I2C_idle_check();
    SSP1CON2bits.SEN = 1;                   //start condition
    I2C_idle_check();
    SSP1BUF = addr << 1;                    //address set
    while(SSP1CON2bits.ACKSTAT == 1);
    I2C_idle_check();
    SSP1BUF = data;                         //send data set
    while(SSP1CON2bits.ACKSTAT == 1);
    I2C_idle_check();
    SSP1CON2bits.PEN = 1;                   //stop condition
}

void interrupt interruption(void) {
    static int counter = 0;
    int data = 0;
    if (TMR4IF == 1) {
        if(counter == 500) {
        /* Timer4 interrupt */
            data = adconv(ADCON_RA0);
            send_data = data >> 2;
            I2C_send_data(ADDR, send_data);
            counter++;
        } else if (counter == 1000) {
            data = adconv(ADCON_RA1);
            send_data = data >> 2;
            I2C_send_data(ADDR, send_data);
            counter = 0;
        } else {
            counter++;
        }
        TMR4IF = 0;             //clear interrupt flag
    }
}

void main(void) {
    init();

    RB3 = 1;
    while(1);
    return;
}

void init(void) {
    OSCCON = 0b01111010;        //Fosc = 16MHz,PLL4x disabled
    TRISA = 0x03;               //RA0 and RA1 input mode, others output mode
    TRISB = 0b00010010;         //RB1(SDA1) and RB4(SCL1) input mode, others output mode
    PORTA = 0x00;               //A pins init
    PORTB = 0x00;               //B pins init
    GIE = 1;                    //all interrupt allowed
    PEIE = 1;                   //peripheral equipment interrupt allowed
    init_adcon(0b00000011, 0x00);
    for(int i = 0; i < 10000; i++);         //wait a while
    init_I2C_master();
    init_Timer4();
}

void init_Timer4(void) {
    T4CON = 0b00000010;         //1:1 postscaler, Timer4 off, prescaler is 16
    PR4 = 249;                  //1kHz timer
    TMR4IE = 1;                 //Timer4 interrupt enabled
    TMR4 = 0;                   //Timer4 init
    TMR4ON = 1;                 //Timer4 on
}

void init_I2C_master(void) {
    SSP1STAT = 0b10000000;      //I2C 100kbps mode
    SSP1CON1 = 0b00111000;      //I2C master mode
    SSP1ADD = 0x27;             //I2C clock 100kHz
    SSP1IE = 1;                 //I2C interrupt enabled
    BCL1IE = 1;                 //I2C bus clash interrupt enabled
    SSP1IF = 0;                 //I2C interrupt flag cleared
    BCL1IF = 0;                 //I2C bus clash interrupt flag cleared
}