/*
 * File:   main.c
 * Author: yuki
 *
 * Created on 2017/03/09, 9:47
 */


#include <xc.h>

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = OFF, CP = OFF, CPD = OFF, BOREN = ON, CLKOUTEN = OFF, IESO = OFF, FCMEN = OFF
#pragma config WRT = OFF, PLLEN = OFF, STVREN = ON, BORV = HI, LVP = OFF

void interrupt_I2C(void);
void interrupt interruption(void);
void init(void);
void init_PWM(void);
void init_Timer4(void);
void init_I2C_slave(char addr);

char received_data[2];

void interrupt_I2C(void) {
    char x;
    static char data_ptr = 0;
    if(SSP1STATbits.R_nW == 0) {
        /* write mode */
        if(SSP1STATbits.D_nA == 0) {
            /* address received */
            data_ptr = 0;
            x = SSP1BUF;
        } else {
            /* data received */
            received_data[data_ptr] = SSP1BUF;
            data_ptr++;
        }
    }
}

void interrupt interruption(void) {
    if(SSP1IF == 1) {
        /* I2C interrupt */
        interrupt_I2C();
        SSP1IF = 0;             //clear interrupt flag
        SSP1CON1bits.CKP = 1;   //open SCL line
    } else if (TMR4IF == 1) {
        /* Timer4 interrupt */
        CCPR3L = received_data[0];
        CCP3CONbits.DC3B = received_data[1] & 0x03;
        TMR4IF = 0;             //clear interrupt flag
    }
}

void main(void) {
    init();
    init_PWM();
    init_Timer4();
    init_I2C_slave(2);
    
    while(1);
    return;
}

void init(void) {
    OSCCON = 0b01111010;        //Fosc = 16MHz,PLL4x disabled
    ANSELA = 0x00;              //all of pins digital mode
    TRISA = 0x00;               //all of A pins output mode
    TRISB = 0b00010010;         //RB1(SDA1) and RB4(SCL1) input mode, others output mode
    PORTA = 0x00;               //A pins init
    PORTB = 0x00;               //B pins init
    GIE = 1;                    //all interrupt allowed
    PEIE = 1;                   //peripheral equipment interrupt allowed
}

void init_PWM(void) {
    CCPTMRS = 0b00000000;
    CCP3CON = 0b00001100;       //single output, PWM mode
    T2CON = 0b00000000;         //1:1 postscaler, Timer2 off, prescaler is 1
    PR2 = 255;                  //PWM resolution 256
    CCPR3L = 0;
    CCPR3H = 0;
    TMR2 = 0;                   //Timer2 init
    TMR2ON = 1;                 //Timer2 on
}

void init_Timer4(void) {
    T4CON = 0b00000010;         //1:1 postscaler, Timer4 off, prescaler is 16
    PR4 = 249;                  //1kHz timer
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