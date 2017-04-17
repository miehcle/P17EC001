/* 
 * File:   i2c.h
 * Author: shu
 *
 * Created on 2017/03/16, 15:09
 */

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
    
    // Slave
    void init_I2C_slave(unsigned char addr);
    void I2C_slave_interrupt(char *received_datas);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

