/* 
 * File:   I2C.c
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 20:31
 */

#include <xc.h>

#include "I2C.h"

/**
 * Initialization
 */
void init_I2C_master(void) {
    SSP1STAT = 0b10000000;      //I2C 100kbps mode
    SSP1CON1 = 0b00101000;      //I2C master mode
    SSP1ADD = 0x27;             //I2C clock 100kHz
    SSP1IE = 1;                 //I2C interrupt enabled
    BCL1IE = 1;                 //I2C bus clash interrupt enabled
    SSP1IF = 0;                 //I2C interrupt flag cleared
    BCL1IF = 0;                 //I2C bus clash interrupt flag cleared
}

/**
 * Idle check
 */
void I2C_idle_check(void) {
    while (( SSP1CON2 & 0x1F ) | (SSP1STAT & 0x05)) ;
}

/**
 * Ack check
 * @return
 * 1: ACK OK
 * 0: ACK NG
 */
int I2C_ack_check(void) {
    unsigned int i;
    for (i = 0; i < 65535; i++) {
        if (SSP1CON2bits.ACKSTAT == 0) {
            return 1;
        }
    }
    return 0;
}

void I2C_send_data(unsigned char addr, unsigned char *data, int length) {
    int i;

    I2C_idle_check();
    SSP1CON2bits.SEN = 1;                   // Start condition

    I2C_idle_check();
    SSP1BUF = addr << 1;                    // Address set

    // Send datas
    for (i=0; i<length; i++) {
        if (!I2C_ack_check()) { return; }       // Ack check
        I2C_idle_check();                       // Idle check
        SSP1BUF = *data;                        // Send data set
        data++;                                 // Pointer increment
        if (!I2C_ack_check()) { return; }       // Ack check
    }
    

    I2C_idle_check();                       // Idle Check
    SSP1CON2bits.PEN = 1;                   // Stop condition

    RA2 = 1;
}