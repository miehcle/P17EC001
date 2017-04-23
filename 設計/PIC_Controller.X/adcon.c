/* 
 * File:   adcon.c
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 18:59
 */

#include <xc.h>

#include "adcon.h"

void init_adcon(int anselA, int anselB){
    ANSELA = anselA;        // Set ANSELA (set ADC pin)
    ANSELB = anselB;        // Set ANSELB (set ADC pin)

    // Set ADCON1
    // Result data will be right justified
    // ADC Conversion Clock = Fosc/8
    // Vref- = AVss
    // Vref+ = AVdd
    ADCON1 = 0b10010000;

    // Set ADCON0
    // ADC is enabled
    // CAUTION: ADCON0 will be changed in adconv()
    ADCON0 = 0b00000001;
}

int adconv(int pin_select){
    int temp;

    // Set ADCON0
    ADCON0 = pin_select;

    // Wait loop
    for (temp = 0; temp < 10; temp++) { }

    GO_nDONE = 1;
    while(GO_nDONE);                // Wait while ADC cycle in progress
    temp = ADRESH;                  // bit 9-8
    temp = (temp << 8) | ADRESL;    // bit 9-8 (OR) bit 7-0
    return temp;
}