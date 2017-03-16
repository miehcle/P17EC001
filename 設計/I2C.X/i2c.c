/* 
 * File:   i2c.c
 * Author: shu
 *
 * Created on 2017/03/16, 15:09
 */

#include <xc.h>
#include "i2c.h"

/*******************************************************************************
 * Master                                                                      *
 * void init_I2C_master(void);                                                 *
 * void I2C_idle_check(void);                                                  *
 * int I2C_ack_check(void);                                                   *
 * void I2C_send_data(unsigned char addr, char *data, int length);             *
 ******************************************************************************/
/**
 * Initialization.
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
 * Idle check.
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
        if (SSP1CON2bits.ACKSTAT == 1) {
            return 1;
        }
    }
    return 0;
}

/**
 * Send data.
 * @param addr
 * @param data
 * @param length
 */
void I2C_send_data(unsigned char addr, char *data, int length) {
    I2C_idle_check();
    SSP1CON2bits.SEN = 1;                   //start condition
    
    I2C_idle_check();
    SSP1BUF = addr << 1;                    //address set   
    
    if (!I2C_ack_check()) { return; }
    
    for(int i = 0; i < length; i++) {
        I2C_idle_check();
        SSP1BUF = *data;                    //send data set
        data++;                             //next data point
        
        if (!I2C_ack_check()) { return; }
    }
    
    I2C_idle_check();
    SSP1CON2bits.PEN = 1;                   //stop condition
}

/*******************************************************************************
 * Slave                                                                       *
 * void init_I2C_slave(unsigned char addr);                                    *
 * void I2C_slave_interrupt(void);                                             *
 *                                                                             *
 * CAUTION:                                                                    *
 * You MUST make interrupt function in your main.c                             *
 ******************************************************************************/
/**
 * Initialization.
 * @param addr
 */
void init_I2C_slave(unsigned char addr) {
    SSP1STAT = 0b10000000;      //I2C 100kbps mode
    SSP1CON1 = 0b00100110;      //I2C slave mode
    SSP1CON2bits.SEN = 1;       //I2C clock stretch enabled
    SSP1ADD = addr << 1;        //I2C address setting
    SSP1IE = 1;                 //I2C interrupt enabled
    BCL1IE = 1;                 //I2C bus clash interrupt enabled
    SSP1IF = 0;                 //I2C interrupt flag cleared
    BCL1IF = 0;                 //I2C bus clash interrupt flag cleared
}

/**
 * Call this function in your main.c (Interrupt).
 */
void I2C_slave_interrupt(char *received_datas) {
    char x;
    static char *data_ptr;
    if(SSP1STATbits.R_nW == 0) {
        /* write mode */
        if(SSP1STATbits.D_nA == 0) {
            /* address received */
            data_ptr = received_datas;
            x = SSP1BUF;
        } else {
            /* data received */
            *data_ptr = SSP1BUF;
            data_ptr++;
        }
    }
}