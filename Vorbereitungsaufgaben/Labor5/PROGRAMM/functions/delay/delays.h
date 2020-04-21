/*
 * delays.h
 *
 *  Created on: Apr 10, 2020
 *      Author: zamza
 */

// simple (?) delays using timer A0!
// works for delays up to 2*(2^16 cycles) / (ACLK freqency) seconds
// Basic idea:
// - put CPU in LPM3 Mode (ACLK still active, which is also set as the source for Timer A0)
// - start timer A0 in up/down mode
// - go into timer's ISR once the count is done
// - return and restore from LPM3
// ACLK should be the clock source for the timer bc of LPM3
// Global interrupts should be enabled
// does not compensate for cycles needed for the interrupts or other commands, so rough timing

#ifndef FUNCTIONS_DELAY_DELAYS_H_
#define FUNCTIONS_DELAY_DELAYS_H_

#include <stdint.h>
#include "functions/timer/timer_A.h"
#include "configurations/clockconfig.h"



void delay_ms(uint16_t ms);
void delay_us(uint16_t us);

#endif /* FUNCTIONS_DELAY_DELAYS_H_ */
