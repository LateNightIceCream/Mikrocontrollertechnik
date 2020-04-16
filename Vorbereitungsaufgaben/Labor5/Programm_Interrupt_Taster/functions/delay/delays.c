/*
 * delays.c
 *
 *  Created on: Apr 10, 2020
 *      Author: zamza
 */

#include "delays.h"


void delay_ms(uint16_t ms) {

    // start timer
    TA0_start_compare_up_down( (ms * ACLK_SPEED_IN_KHZ/ACLK_DIVIDER) / 2 );

    // enter low power mode 3
    LPM0;

    // exiting from low power mode
    //stop the timer
    TA0_STOP;

}

// up mode to get lower
// clock speed should be high (8mhz) to get accurate results!
void delay_us(uint16_t us) {

    // start timer
    TA0_start_compare_up( (us * ACLK_SPEED_IN_KHZ/ACLK_DIVIDER) / 1000 );

    // enter low power mode 3
    LPM0;

    // exiting from low power mode
    //stop the timer
    TA0_STOP;

}

#pragma vector=TIMER0_A0_VECTOR // assign vector address
__interrupt void Timer_A0_interrupt(void) {

    //P1OUT ^= BIT0;

    // modify the SR Register that is on the stack to get out of low power mode
    LPM0_EXIT;




}
