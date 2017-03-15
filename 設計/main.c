/*
 * File:   main.c
 * Author: shu
 *
 * Created on 2017/03/07, 16:24
 */

#include <xc.h>

/* pragma */
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

/* macro */
#define _XTAL_FREQ  8000000

#define ON 1            //segment => ON
#define OFF 0           //segment => OFF
#define OPEN 1          //digit   => OPEN
#define CLOSE 0         //digit   => CLOSE
#define MAX_VALUE 5.0   //
#define RATIO 1.0       //

//ADCON0
//bit7  :Unimplemented[0]
//bit6-2:CHS [00011]=AN4
//bit1  :GO_nDONE[1]
//bit0  :ADON    [1]=ADC is enabled
#define NUM_READ_PIN 0b00010011

/* global variables */
int led_disp[12][7]=
{
    {1,1,1,1,1,1,0},    //0
    {0,1,1,0,0,0,0},    //1
    {1,1,0,1,1,0,1},    //2
    {1,1,1,1,0,0,1},    //3
    {0,1,1,0,0,1,1},    //4
    {1,0,1,1,0,1,1},    //5
    {0,0,1,1,1,1,1},    //6
    {1,1,1,0,0,0,0},    //7
    {1,1,1,1,1,1,1},    //8
    {1,1,1,1,0,1,1},    //9
    {0,0,0,0,0,0,0},    //none
    {1,0,0,1,1,1,1}     //E
};

/* function proto type */
void init(void);
void loop(void);
int ad_conv(int pin_select);
double da_conv(int analog);
void open(int select);
void close();
void wait(unsigned int wait_time);
void disp(double num);
void set_disp(int disp[7], int dp);

/* functions */
void set_A(int on_off) {RB4 = on_off;}
void set_B(int on_off) {RB5 = on_off;}
void set_C(int on_off) {RB6 = on_off;}
void set_D(int on_off) {RB7 = on_off;}
void set_E(int on_off) {RA6 = on_off;}
void set_F(int on_off) {RA7 = on_off;}
void set_G(int on_off) {RA0 = on_off;}
void set_DP(int on_off) {RA1 = on_off;}
void set_BIT0(int open_close) {RA3 = open_close;}
void set_BIT1(int open_close) {RA2 = open_close;}

int main(void) {
    init();
    while(1) {
        loop();
    }
    return 0;
}

void init(void) {
     OSCCON = 0b01110010;
     ANSELA = 0b00001000;
     ANSELB = 0b00000000;
     TRISA  = 0b00001000;
     TRISB  = 0b00000000;
     PORTA  = 0b00000000;
     PORTB  = 0b00000000;
     
     ADCON1 = 0b10010000;
}

void loop(void) {
    disp(da_conv(ad_conv(NUM_READ_PIN)));
}

int ad_conv(int pin_select) {
     int temp;

     ADCON0 = pin_select;
     
     while(GO_nDONE);
     temp = ADRESH;
     temp = ( temp << 8 ) | ADRESL;

     return temp;
}

double da_conv(int digital) {
    double analog;
    analog = digital * MAX_VALUE * RATIO / 1024;
    return analog;
}

void wait(unsigned int wait_time) {
    int i;
    for (i=0; i<wait_time; i++) {
        __delay_ms(1);
    }
}

void set_disp(int disp[7], int dp) {
    set_A(disp[0]);
    set_B(disp[1]);
    set_C(disp[2]);
    set_D(disp[3]);
    set_E(disp[4]);
    set_F(disp[5]);
    set_G(disp[6]);
    set_DP(dp);
}

void disp(double num) {
    int digits[4];
    int i;
    int dp;
    int dp_index=-1;
    int over=0;
    
    if (num >= 10000) {
        digits[3] = 10;
        digits[2] = 10;
        digits[1] = 10;
        digits[0] = 11;
        over = 1;
    } else if (num >= 1000) {
        
    } else if (num >= 100) {
        num *= 10;
        dp_index = 1;
    } else if (num >= 10) {
        num *= 100;
        dp_index = 2;
    } else { 
        num *= 1000;
        dp_index = 3;
    }

    if (!over) {
        digits[3] = (int) (num/1000)%10;
        digits[2] = (int) (num/100)%10;
        digits[1] = (int) (num/10)%10;
        digits[0] = (int) (num)%10;
    }
    
    for (i=0; i<4; i++) {
        dp = ((i==dp_index) ? (1:0));
        set_disp(led_disp[digits[i]],dp);
        open(i);
        wait(3);
    }
    close();
}

void open(int select) {
    close();
    switch (select) {
        case 0:
            set_BIT0(OPEN);
            break;
        case 1:
            set_BIT1(OPEN);
            break;
        default:
            break;
    }
}

void close() {
    set_BIT0(CLOSE);
    set_BIT1(CLOSE);
}