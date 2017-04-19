/* 
 * File:   controller.h
 * Author: isaka-PC
 *
 * Created on 2017/04/17
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#ifdef	__cplusplus
extern "C" {
#endif

/* controler value */
#define KP 1.0       //proportional controler value
#define KI 0.01      //integral controler value
#define KD 0.0       //differential controler value

#define CURRENT_MAX_A 12000        //[mA]
#define CURRENT_LIMIT_A 5000       //[mA]
#define CURRENT_MAX_BIN AMPARE_TO_BINARY(CURRENT_MAX_A)     //Binary
#define CURRENT_LIMIT_BIN AMPARE_TO_BINARY(CURRENT_LIMIT_A) //Binary


#define MAX(A, B) ((A>=B) ? (A) : (B))
#define MIN(A, B) ((A<=B) ? (A) : (B))
#define CLIP(x, TOP, BOTTOM) MIN(MAX(x, BOTTOM), TOP)       //BOTTOM <= x <= TOP calculation

/* current[mA] = current(binary) * CURRENT_MAX / 0x0400 */
#define AMPARE_TO_BINARY(current) ((((long) current) * ((long) CURRENT_MAX_A)) >> 10)
/* For Display */
#define BINARY_TO_BINARY(binary) ((int) (binary * ((float) CURRENT_MAX_BIN / CURRENT_LIMIT_BIN)))


void measure(int* value, int pin_select);
void calc(int ref, int mes,int *output);
int P_calc(int res);
int I_calc(int res);
int D_calc(int res);
void output(int out);
void interrupt_controller(void);
void init_PWM(void);

// current conversion (Depend on Current Sensor)
int current_conversion(int measured_value);

// getter/setter
void set_current_limit(int current_limit);
int get_current_limit(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLLER_H */

