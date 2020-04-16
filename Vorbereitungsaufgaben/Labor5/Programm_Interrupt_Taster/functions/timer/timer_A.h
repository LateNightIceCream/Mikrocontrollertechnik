/*
 * timer_A.h
 *
 *  Created on: Apr 6, 2020
 *      Author: zamza
 */

#ifndef TIMER_A_H_
#define TIMER_A_H_

#include <msp430.h>
#include <stdint.h>

#define TA0_STOP TA0CTL &= ~(MC0 | MC1); // puts the timer in stop mode

void TA0_init(uint16_t clocksource, uint16_t ID_divide, uint16_t TAIDEX_divide);

void TA0_start_compare_cont(void);
void TA0_start_compare_up(uint16_t cycles);
void TA0_start_compare_up_down(uint16_t cycles);

#endif /* TIMER_A_H_ */
