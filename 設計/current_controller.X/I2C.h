/* 
 * File:   I2C.h
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 20:31
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CURRENT_MAX 12.0    // [A] 255(data) = 12(display)

void init_I2C_master(void);
void I2C_idle_check(void);
int I2C_ack_check(void);
void I2C_send_data(unsigned char addr, unsigned char *data, int length);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

