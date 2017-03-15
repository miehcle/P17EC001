/* 
 * File:   display.h
 * Author: shu
 *
 * Created on 2017/03/13, 23:34
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define ON 1
#define OFF 0
    
#define A     0b00000001
#define B     0b00000010
#define C     0b00000100
#define D     0b00001000
#define E     0b00010000
#define F     0b00100000
#define G     0b01000000
#define P     0b10000000
    
#define ZERO  0b00111111
#define ONE   0b00000110
#define TWO   0b01011011
#define THREE 0b01001111
#define FOUR  0b01100110
#define FIVE  0b01101101
#define SIX   0b01111101
#define SEVEN 0b00100111
#define EIGHT 0b01111111
#define NINE  0b01101111
#define NONE  0b00000000
#define ERROR 0b01111001
    
#define DOT   0b10000000
    
//    void light1(int digit,    int dot);
    void light2(int digit[2], int dot[2]);
//    void light3(int digit[3], int dot[3]);
//    void light4(int digit[4], int dot[4]);
//    void error1();
    void error2();
//    void error3();
//    void error4();
    void set_num(int num);
    void set_a(int on_off);
    void set_b(int on_off);
    void set_c(int on_off);
    void set_d(int on_off);
    void set_e(int on_off);
    void set_f(int on_off);
    void set_g(int on_off);
    void set_p(int on_off);
//    void open1();
    void open2(int digit_select);
//    void open3(int digit_select);
//    void open4(int digit_select);
//    void close1();
    void close2();
//    void close3();
//    void close4();
    void wait(unsigned int wait_time);

#ifdef	__cplusplus
}
#endif

#endif	/* DISPLAY_H */

