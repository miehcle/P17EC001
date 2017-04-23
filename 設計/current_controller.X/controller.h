/* 
 * File:   controller.h
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 21:50
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#ifdef	__cplusplus
extern "C" {
#endif

// Macro
#define MAX(a,b) ((a) > (b) ? (a):(b))                              // Return bigger one
#define MIN(a,b) ((a) > (b) ? (b):(a))                              // Return smaller one
#define CLIP(bottom,x,top) (MAX( MIN( (x) , (top) ), (bottom) ))    // Return bottom <= x <= top

#define KP 7
#define KI 0
#define KD 0
#define FILTER_ORDER 5

#define SAFETY_PIN RA6
#define VOLUME_PIN ADCON_RA0    // Volume ADC pin
#define CURRENT_PIN ADCON_RA1   // Current ADC pin

int safety_check();
void measure(int *volume, float *current);
void PID(int ref, int mes, int *output);
int calc_P(int res);
int calc_I(int res);
int calc_D(int res);

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROLLER_H */

