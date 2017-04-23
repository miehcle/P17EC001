/* 
 * File:   main.c
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 18:22
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "adcon.h"
#include "pwm.h"
#include "I2C.h"
#include "controller.h"

#define DEBUG
#define DISPLAY_ADDR (0x02)     // Slave address for Display

/* global */
int power = 0;
int volume = 0;
float current = 0.0;

/*
 * main
 *
 */
int main(void) {
    init();

    while(1) {
        loop();
    }

    return (EXIT_SUCCESS);
}

void init(void) {
    OSCCON = 0b01111010;        // Internal clock 16MHz
    ANSELA = 0b00000011;        // RA0, RA1 = Analog INPUT
    ANSELB = 0b00000000;        // RBx = Digital I/O
    TRISA  = 0b11000011;        // RA0, RA1, RA6, RA7 = INPUT, others = OUTPUT
    TRISB  = 0b00010010;        // RB1(SDA1) and RB4(SCL1) = INPUT, others = OUTPUT
    PORTA  = 0b00000000;        // Initializes output pin as LOW.
    PORTB  = 0b00000000;        // Initializes output pin as LOW.

    init_adcon(ANSEL_A, ANSEL_B);   // call init_adcon()    (See adcon.h & adcon.c)
    init_PWM();                     // call init_pwm()      (See pwm.h & pwm.c)
    init_I2C_master();              // call init_I2C_master (See I2C.h & I2c.c)
    init_Timer2();                  // call init_Timer2()
    init_Timer4();                  // call inti_Timer4()
    init_interruption();            // call init_interruption()

    RB3 = 1;                        // I2C Power ON
}

/**
 * For PWM output.
 */
void init_Timer2(void) {
    T2CON = 0b00000000;         // 1:1 Postscaler, Prescaler is 1
    TMR2 = 0b00000000;          // Initialize Timer2
    TMR2ON = 1;                 // Timer2 ON
}

/**
 * For I2C Communication
 */
void init_Timer4(void) {
    T4CON = 0b00000001;         // 1:1 Postscaler, Prescaler is 4
    TMR4 = 0b00000000;          // Initialize Timer4
    TMR4ON = 1;                 // Timer4 ON
}

void init_interruption(void) {
    TMR2IE = 1;                 // Timer2 interruption enabled
    TMR2IF = 0;                 // Set Timer2 interruption flag as 0
    TMR4IE = 1;                 // Timer4 interruption enabled
    TMR4IF = 0;                 // Set Timer4 interruption flag as 0
    GIE = 1;                    // all interruption allowed
    PEIE = 1;                   // peripheral equipment interrupt allowed
}

void loop(void) {
    
}

void interrupt inter(void) {
    static int cnt_tmr2 = 0;
    static int cnt_tmr4 = 0;

    if (TMR2IF == 1) {
        if (cnt_tmr2++ > 8) {
            cnt_tmr2 = 0;
            if (safety_check()) {
                // RUN
                measure(&volume, &current);
                PID(volume, 0, &power);
            }
            else {
                // STOP
                power = 0;
            }
            output_PWM(power);
        }
        TMR2IF = 0;
    }

    if (TMR4IF == 1) {
        if (cnt_tmr4++ > 1024) {
            cnt_tmr4 = 0;
            char data = (power >> 2) & 0xFF;
            I2C_send_data(DISPLAY_ADDR, &data, 1);
        }
        TMR4IF = 0;
    }
}

