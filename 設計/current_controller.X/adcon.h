/* 
 * File:   adcon.h
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 18:58
 */

#ifndef ADCON_H
#define	ADCON_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * ADCON0 pin_select data
 * You must call init_adcon and initialize ANSELA and ANSELB to use analog mode.
 */
#define ADCON_RA0 0b00000001         //AN0
#define ADCON_RA1 0b00000101         //AN1
#define ADCON_RA2 0b00001001         //AN2
#define ADCON_RA3 0b00001101         //AN3
#define ADCON_RA4 0b00010001         //AN4
#define ADCON_RB1 0b00101101         //AN11
#define ADCON_RB2 0b00101001         //AN10
#define ADCON_RB3 0b00100101         //AN9
#define ADCON_RB4 0b00100001         //AN8
#define ADCON_RB5 0b00011101         //AN7
#define ADCON_RB6 0b00010101         //AN5
#define ADCON_RB7 0b00011001         //AN6

void init_adcon(int anselA, int anselB);
int adconv(int pin_select);

#ifdef	__cplusplus
}
#endif

#endif	/* ADCON_H */

