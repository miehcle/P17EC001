/*
 * File:   main.c
 * Author: shu
 *
 * Created on 2017/01/13, 0:43
 */


#include <xc.h>

#define _XTAL_FREQ  8000000
#define MAX_VOLTAGE 5.0
#define VOLTAGE_RATIO 1.0

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

void disp(double num);
void set_disp(int disp[7], int dp);
void disp_a(int sw);
void disp_b(int sw);
void disp_c(int sw);
void disp_d(int sw);
void disp_e(int sw);
void disp_f(int sw);
void disp_g(int sw);
void disp_dp(int sw);
void open(int select);
void close();
void loop();
void wait(unsigned int wait_time);
void init();
int adconv();
double daconv(int digital);

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

void main(void) {
    init();
    
    while(1) {
        loop();
    }
    
    return;
}

void init() {
     OSCCON = 0b01110010;
     ANSELA = 0b00010000;
     ANSELB = 0b00000000;
     TRISA  = 0b00010000;
     TRISB  = 0b00000000;
     PORTA  = 0b00000000;
     PORTB  = 0b00000000;
     
     ADCON1 = 0b10010000;
     ADCON0 = 0b00010011;
     __delay_us(5);
}

void loop() {
    disp(daconv(adconv()));
}

void wait(unsigned int wait_time) {
    int i;
    for (i=0; i<wait_time; i++) {
        __delay_ms(1);
    }
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
        dp = ((3-i==dp_index) ? (1:0));
        set_disp(led_disp[digits[3-i]],dp);
        open(i);
        wait(3);
    }
    close();
}

void set_disp(int disp[7], int dp) {
    disp_a(disp[0]);
    disp_b(disp[1]);
    disp_c(disp[2]);
    disp_d(disp[3]);
    disp_e(disp[4]);
    disp_f(disp[5]);
    disp_g(disp[6]);
    disp_dp(dp);
}

void disp_a(int sw) {RB4=sw;}
void disp_b(int sw) {RB5=sw;}
void disp_c(int sw) {RB6=sw;}
void disp_d(int sw) {RB7=sw;}
void disp_e(int sw) {RA6=sw;}
void disp_f(int sw) {RA7=sw;}
void disp_g(int sw) {RA0=sw;}
void disp_dp(int sw) {RA1=sw;}
void open(int select) {
    close();
    switch (select) {
        case 0:
            RA2=1;
            break;
        case 1:
            RA3=1;
            break;
        case 2:
            //RA4=1;
            break;
        case 3:
            RA1=1;
            break;
        default:
            break;
    }
}
void close() {
    RA2=0;
    RA3=0;
    //RA4=0;
    RA1=0;
}

int adconv()
{
     int temp;

     GO_nDONE = 1;
     while(GO_nDONE);
     temp = ADRESH;
     temp = ( temp << 8 ) | ADRESL;

     return temp;
}

double daconv(int digital) {
    double analog;
    analog = digital * MAX_VOLTAGE * VOLTAGE_RATIO / 1024;
    return analog;
}