/*
 * File:   main.c
 * Author: yuki
 *
 * Created on 2017/03/11, 10:30
 */


#include <xc.h>

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = OFF, CP = OFF, CPD = OFF, BOREN = ON, CLKOUTEN = OFF, IESO = OFF, FCMEN = OFF
#pragma config WRT = OFF, PLLEN = OFF, STVREN = ON, BORV = HI, LVP = OFF

#define RA0_PIN 0b00000011   //RA0pin
#define RA1_PIN 0b00000111    //RA1pin
#define ADDR 0x02

int adconv(int pin_select);
void I2C_idle_check(void);
void I2C_send_data(char addr, unsigned char data);
void interrupt interruption(void);
void init(void);
void init_Timer4(void);
void init_I2C_master(void);

unsigned char send_data = 0;

int adconv(int pin_select) {
    int temp;
    
    ADCON0 = pin_select;
    while(GO_nDONE);
    temp = ADRESH;
    temp = (temp << 8) | ADRESL;
    return temp;
}

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
    if (TMR4IF == 1) {
        /* Timer4 interrupt */
        int data = adconv(RA0_PIN);
        send_data = data >> 2;
        I2C_send_data(ADDR, send_data);
        TMR4IF = 0;             //clear interrupt flag
    }
}

void main(void) {
    init();
    init_Timer4();
    init_I2C_master();
    
    while(1);
    return;
}

void init(void) {
    OSCCON = 0b01111010;        //Fosc = 16MHz,PLL4x disabled
    ANSELA = 0x03;              //RA0 and RA1 analog mode, others digital mode
    TRISA = 0x03;               //RA0 and RA1 input mode, others output mode
    TRISB = 0b00010010;         //RB1(SDA1) and RB4(SCL1) input mode, others output mode
    PORTA = 0x00;               //A pins init
    PORTB = 0x00;               //B pins init
    ADCON1 = 0b10010000;
    ADCON0 = 0x01;              //A/D converter setting
    GIE = 1;                    //all interrupt allowed
    PEIE = 1;                   //peripheral equipment interrupt allowed
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
    SSP1CON1 = 0b00101000;      //I2C slave mode
    SSP1ADD = 0x27;        //I2C clock 100kHz
    SSP1IE = 1;                 //I2C interrupt enabled
    BCL1IE = 1;                 //I2C bus clash interrupt enabled
    SSP1IF = 0;                 //I2C interrupt flag cleared
    BCL1IF = 0;                 //I2C bus clash interrupt flag cleared
}