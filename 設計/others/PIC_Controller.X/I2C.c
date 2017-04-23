/* 
 * File:   I2C.c
 * Author: isaka-PC
 *
 * Created on 2017/04/17
 */

#include "I2C.h"
#include "controller.h"
#include "config.h"

#define SLAVE_DISPLAY_ADDR (0x02)

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

    init_Timer4();              //Timer4 initialize
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

/*
 * Send data per 1 sec.
 */
void interrupt_I2C(void) {
    static int counter = 0;
    if (TMR4IF == 1) {
        if(counter < 250) {
            counter++;
        } else {
            /* Timer4 interrupt */
            int data = get_current_limit();
            char send_data = data >> 2;
            I2C_send_data(SLAVE_DISPLAY_ADDR, &send_data, 1);
            counter = 0;
        }
        TMR4IF = 0;             //clear interrupt flag
    }
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

/*******************************************************************************
 * Timer                                                                       *
 * void init_Timer4(void);                                                     *
 *                                                                             *
 * CAUTION:                                                                    *
 * You MUST make interrupt function in your main.c                             *
 ******************************************************************************/

void init_Timer4(void) {
    T4CON = 0b00000010;         //1:1 postscaler, Timer4 off, prescaler is 16
    PR4 = 249;                  //1kHz timer
    TMR4IE = 1;                 //Timer4 interrupt enabled
    TMR4 = 0;                   //Timer4 init
    TMR4ON = 1;                 //Timer4 on
}

void call_send(void) {
    int data = get_current_limit();
    char send_data = data >> 2;
    I2C_send_data(SLAVE_DISPLAY_ADDR, &send_data, 1);
}