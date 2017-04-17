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

#define MAX(int A, int B) ((A>=B) ? (A) : (B))
#define MIN(int A, int B) ((A<=B) ? (A) : (B))
#define CLIP(int x, int TOP, int BOTTOM) MIN(MAX(x, BOTTOM), TOP)       //BOTTOM <= x <= TOP calculation

void measure(int* value, int pin_select);
void calc(int ref, int mes,int *output);
int P_calc(int res);
int I_calc(int res);
int D_calc(int res);
void output(int output);
void interrupt_controller(void);
void init_PWM(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLLER_H */

