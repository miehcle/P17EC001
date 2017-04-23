/* 
 * File:   controller.c
 * Author: isaka-PC
 *
 * Created on 2017/04/23, 21:50
 */

#include <xc.h>

#include "controller.h"
#include "adcon.h"

/**
 * return = 0: DO NOT RUN MOTOR
 * return = 1: RUN MOTOR
 * @return
 */
int safety_check() {
    // CAUTION: SW = PULL UP
    if (SAFETY_PIN == 1) {
        return 0;
    }
    else {
        return 1;
    }
}

/**
 * volume : as-is
 * current: depend on current sensor
 * 
 * @param volume
 * @param current
 */
void measure(int *volume, float *current) {
    *volume = adconv(VOLUME_PIN);
    int current_sens = adconv(CURRENT_PIN);

    /* CHANGE */
    *current = 0 * current_sens;
    /* CHANGE */
}

void PID(int ref, int mes, int *power) {
    int temp = 0;
    int res = ref - mes;

    temp = calc_P(res) + calc_I(res) + calc_D(res);

    *power = CLIP(0, temp, 255);

    /* CHANGE */
    *power = ref;
    /* CHANGE */
}

int calc_P(int res) {
    int p = res * KP;

    return p;
}

int calc_I(int res) {
    static long long int i = 0;

    i += res * KI;

    return i;
}

int calc_D(int res) {
    static int d = 0;
    static int d_bef = 0;
    static int d_sum = 0;

    d_sum = d_sum + d_bef;
    d = (res * KD - d_sum) * FILTER_ORDER;
    d_bef = d;

    return d;
}