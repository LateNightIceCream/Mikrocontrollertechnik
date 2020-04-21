/*
 * timer_A.h
 *
 *  Created on: Apr 6, 2020
 *      Author: R. Grünert
 */

#ifndef TIMER_A_H_
#define TIMER_A_H_

#include <msp430.h>
#include <stdint.h>

#define TA0_STOP TA0CTL &= ~(MC0 | MC1); // puts the timer in stop mode
#define TA1_STOP TA1CTL &= ~(MC0 | MC1);

void TA0_init(uint16_t clocksource, uint16_t ID_divide);

void TA0_start_compare_cont(void);
void TA0_compare_up(uint16_t cycles, uint8_t ccrx);
void TA0_start_compare_up_down(uint16_t cycles);


void TA1_init(uint16_t clocksource, uint16_t ID_divide);


#endif /* TIMER_A_H_ */
