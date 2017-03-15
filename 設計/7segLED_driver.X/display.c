/* 
 * File:   display.c
 * Author: shu
 *
 * Created on 2017/03/13
 */

#include <xc.h>
#include "display.h"

// For using __delay_ms()
#define _XTAL_FREQ  8000000

/**
 * Call this function.
 * 
 * When you throw {0, 2} as digit and {OFF, ON} as digit[1] to this function,
 * you will see [2.0]
 * @param digit
 * @param dotm.,
 */
void light2(int digit[2], int dot[2]) {
    int i;
    int num;
    for (i=0; i<2; i++) {
        num = 0;
        
        // Set number
             if (digit[i] ==  0) { num =  ZERO; }
        else if (digit[i] ==  1) { num =   ONE; }
        else if (digit[i] ==  2) { num =   TWO; }
        else if (digit[i] ==  3) { num = THREE; }
        else if (digit[i] ==  4) { num =  FOUR; }
        else if (digit[i] ==  5) { num =  FIVE; }
        else if (digit[i] ==  6) { num =   SIX; }
        else if (digit[i] ==  7) { num = SEVEN; }
        else if (digit[i] ==  8) { num = EIGHT; }
        else if (digit[i] ==  9) { num =  NINE; }
        else if (digit[i] == 10) { num =  NONE; }
        else if (digit[i] == 11) { num = ERROR; }
        else                     { num = ERROR; }
        
        // Add dot
        if (dot[i] == ON) { num += DOT; }
        
        set_num(num);       // Set ports
        open2(i);           // Open port
        wait(5);            // Wait
        close2();           // Close port
    }
}

/**
 * Call this function.
 * 
 * You will see [E E]
 */
void error2() {
    int error_digit[2] = { 11, 11 };
    int error_dot[2] = { ON, ON };
    light2(error_digit, error_dot);
}

/**
 * You don't have to warry about this function.
 * @param num
 */
void set_num(int num) {
    set_a((num & A));
    set_b((num & B) >> 1);
    set_c((num & C) >> 2);
    set_d((num & D) >> 3);
    set_e((num & E) >> 4);
    set_f((num & F) >> 5);
    set_g((num & G) >> 6);
    set_p((num & P) >> 7);
}

/**
 * You don't have to warry about this function.
 * @param on_off
 */
void set_a(int on_off) { RB2 = on_off; }
void set_b(int on_off) { RB5 = on_off; }
void set_c(int on_off) { RB6 = on_off; }
void set_d(int on_off) { RB7 = on_off; }
void set_e(int on_off) { RB1 = on_off; }
void set_f(int on_off) { RA7 = on_off; }
void set_g(int on_off) { RA0 = on_off; }
void set_p(int on_off) { RA1 = on_off; }

/**
 * You don't have to warry about this function.
 * @param digit_select
 */
void open2(int digit_select) {
    close2();
    if (digit_select == 0) { RA3 = ON; }    // digit0 ON
    if (digit_select == 1) { RA2 = ON; }    // digit1 ON
}

/**
 * You don't have to warry about this function.
 */
void close2() {
    RA3 = OFF;  // digit0 off
    RA2 = OFF;  // digit1 off
}

/**
 * You can call this function.
 * @param wait_time
 */
void wait(unsigned int wait_time) {
    int i;
    for (i=0; i<wait_time; i++) {
        __delay_ms(1);
    }
}