/*
 * File:   main.c
 * Author: yuki
 *
 * Created on 2017/03/07, 11:28
 */


#include <xc.h>
#include "adcon.h"

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, MCLRE = OFF, CP = OFF, CPD = OFF, BOREN = ON, CLKOUTEN = OFF, IESO = OFF, FCMEN = OFF
#pragma config WRT = OFF, PLLEN = OFF, STVREN = ON, BORV = HI, LVP = OFF

/* controler value */
#define KP 1.0       //proportional controler value
#define KI 0.01      //integral controler value
#define KD 0.0       //differential controler value

#define MAX(int A, int B) ((A>=B) ? (A) : (B))
#define MIN(int A, int B) ((A<=B) ? (A) : (B))
#define CLIP(int x, int TOP, int BOTTOM) MIN(MAX(x, BOTTOM), TOP)       //BOTTOM <= x <= TOP calculation

void measure(int* value, int pin_select);
void calc(int ref, int mes,int *output);
int P_calc(int res);
int I_calc(int res);
int D_calc(int res);
void output(int output);
void interrupt controler(void);
void init(void);

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
    *output = CLIP(output, 1023, 0);
}

int P_calc(int res) {
    int temp;
    
    temp = res * KP;
    return temp;
}

int I_calc(int res) {
    int temp;
    static int sum = 0;
    
    sum += res;
    sum = CLIP(sum, 1023, 0);
    temp = sum * KI;
    return temp;
}

int D_calc(int res) {
    int temp;
    static int previous = 0;
    
    temp = (res - previous) * KD;
    previous = res;
    return temp;
}

/* PWM width output */
void output(int output) {
    CCPR3L = output >> 2;
    CCP3CONbits.DC3B = output & 0x03;
}

/* timer interrupt */
void interrupt controler(void) {
    if(TMR2IF == 1) {
        int current;
        int volume;
        int output;
        measure(&current, ADCON_RA1);
        measure(&volume, ADCON_RA0);
        calc(current,volume,&output);
        output(output);
        TMR2IF = 0;
    }

}

void main(void) {
    init();
    
    while(1);
    return;
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