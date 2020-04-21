/*
 * delays.c
 *
 *  Created on: Apr 10, 2020
 *      Author: R. Grünert
 */

#include "delays.h"

///////////////////////////////////////////////////////
/// for short periods preferably use __delay_cycles()
/// pretty bad accuracy at low time values,
/// clock dependent
///////////////////////////////////////////////////////

/// delay_ms(ms)
/// delays ms ms by turning CPU off
/// and waiting for a timer interrupt
///////////////////////////////////////////////////////

void delay_ms(uint16_t ms) {

    // set cycle count in TA0CCR0
        TA1CCR0 = ms*CYCLE_FACTOR_MS;  // does not account for other op cycles needed

    // start up-down-mode
        TA1CTL |= (MC0 | MC1);

     // enter low power mode 3, exit after Timer ISR
        LPM0;

}

/// delay_us(us)
/// delays us us by turning CPU off
/// and waiting for a timer interrupt
///////////////////////////////////////////////////////

void delay_us(uint16_t us) {

   // set cycle count in TA0CCR0
       TA1CCR0 = us*CYCLE_FACTOR_US;

   // start up-mode
       TA1CTL |= MC0;

    // enter low power mode 3, exit after Timer ISR
       LPM0;

}

/// TA1 ISR
///////////////////////////////////////////////////////

//#pragma vector=TIMER0_A0_VECTOR // assign vector address
#pragma vector=TIMER1_A1_VECTOR // timer A0 TA0IV
__interrupt void Timer_A1_interrupt(void) {

    // reset TA Interrupt flag
    TA1CTL &= ~TAIFG;

    //stop the timer
    TA1_STOP;

    // modify the SR Register that is on the stack to get out of low power mode
    LPM0_EXIT;

}
