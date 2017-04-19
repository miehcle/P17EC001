/* 
 * File:   I2C.h
 * Author: isaka-PC
 *
 * Created on 2017/04/17, 15:56
 */

/*Include Guard*/
#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

// Master
void init_I2C_master(void);
void I2C_idle_check(void);
int I2C_ack_check(void);
void I2C_send_data(unsigned char addr, char *data, int length);
void interrupt_I2C(void);

// Slave
void init_I2C_slave(unsigned char addr);
void I2C_slave_interrupt(char *received_datas);

// Timer
void init_Timer4(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

