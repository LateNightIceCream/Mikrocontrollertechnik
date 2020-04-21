/*
 * delays.h
 *
 *  Created on: Apr 10, 2020
 *      Author: R. Grünert
 */

// simple (?) delays using timer A0!
// Basic idea:
// - put CPU in LPM3 Mode (ACLK still active, which is also set as the source for Timer A0)
// - start timer A1 in up/down mode
// - go into timer's ISR once the count is done
// - return and restore from LPM3
// Global interrupts should be enabled
// does not compensate for cycles needed for the interrupts or other commands, so rough timing at low delay times

#ifndef FUNCTIONS_DELAY_DELAYS_H_
#define FUNCTIONS_DELAY_DELAYS_H_

/// INCLUDES
/////////////////////
#include <functions/timer_old/timer_A.h>
#include <stdint.h>
#include "configurations/clockconfig.h"


/// CONSTANTS + MACROS
///////////////////////////////////////////////////////

// cycle factors for calculating the timer cycles/counts, outsourced to reduce cycle need
#define CYCLE_FACTOR_US         (ACLK_SPEED_IN_KHZ/(1000))
#define CYCLE_FACTOR_MS         (ACLK_SPEED_IN_KHZ/2)

// burning cpu cycles method, delay_ms and delay_us should be used for longer time delays, since they cant get too low
#define TEN_MICRO_SECOND_CYCLES (MCLK_SPEED_IN_KHZ)/100
// for 10 us and below this functions uses less cycles than the delay using timer function @ 1MHz. Always depends on Clockfrequency!
#define DELAY_10US              __delay_cycles( (TEN_MICRO_SECOND_CYCLES - 8)); // about 8 cycles needed for __delay_cycles() itself


/// FUNCTIONS
///////////////////////////////////////////////////////

void delay_ms(uint16_t ms);
void delay_us(uint16_t us);


#endif /* FUNCTIONS_DELAY_DELAYS_H_ */
