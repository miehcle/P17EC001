/* 
 * File:   pwm.c
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 19:21
 */

#include <xc.h>

#include "pwm.h"

/**
 * Use CCP3(RA3)
 */
void init_PWM(void) {
    CCPTMRS = 0b00000000;   // CCP3 is based off Timer 2 in PWM Mode
    CCP3CON = 0b00001100;   // PWM Mode
    CCPR3L = 0;             // Initialize CCP3L register
    CCPR3H = 0;             // Initialize CCP3H register
    PR2 = 255;              // PWM Period = (PR2 + 1) * 4 * Tosc * (TMR2 Prescale Value)   where (Tosc = 1/Fosc)
}

/**
 * Change the Pulse width.
 *
 * NOTE:
 * PWM Period = (PR2 + 1) * 4 * Tosc * (TMR2 Prescale Value)   where (Tosc = 1/Fosc)
 * Pulse width = (CCP3L:CCP3CON<5:4>) * Tosc * (TMR2 Prescale Value)
 * Duty Cycle Ratio = Pulse_Width / PWM_Period =  (CCP3L:CCP3CON<5:4>) / (4 * (PR2 + 1))
 * @param out
 */
void output_PWM(int out) {
    CCPR3L = out >> 2;              // Set CCP3L as eight MSbs of 'out'
    CCP3CONbits.DC3B = out & 0x03;  // Set DC3B(CCP3CON<5:4>) as two LSbs of 'out'
}