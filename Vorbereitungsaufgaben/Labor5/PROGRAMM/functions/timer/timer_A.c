/*
 * timer_A.c
 *
 *  Created on: Apr 6, 2020
 *      Author: zamza
 */

#include "timer_A.h"

void TA0_init(uint16_t clocksource, uint16_t ID_divide, uint16_t TAIDEX_divide) {

    // total divide is ID_divide * TAIDEX_divide

    // TIMER CLOCK SOURCE
        // reset any previous setting
        TA0CTL &= ~(TASSEL_3);
        // write new setting
        TA0CTL |= clocksource;

    // CLOCK DIVIDERS
        // set timer clock dividers
        TA0CTL &= ~(ID_3);
        TA0CTL |= ID_divide;

        TA0EX0 &= ~(TAIDEX_7);
        TA0EX0 |= TAIDEX_divide;

        // clear TAR / reset clock divider logic
        TA0CTL |= TACLR;

}

// start continuous mode
void TA0_start_compare_cont(void) {

    // disable TAIFG interrupts
        TA0CTL   &= ~TAIE;
    // enable CCIFG interrupts
        TA0CCTL0 |= CCIE; // capture/compare interrupt

    // set timer mode to compare continuous
        TA0_STOP

        TA0CCTL0 &= ~CAP; // compare mode
        TA0CTL   |= MC1;  // continuous mode

}

// start up mode
void TA0_start_compare_up(uint16_t cycles) {

    // disable TAIFG interrupts
        TA0CTL   &= ~TAIE;
    // enable CCIFG interrupts
        TA0CCTL0 |= CCIE; // capture/compare interrupt

    // set timer mode to compare value
        TA0_STOP

    // set value to count in TA0CCR0
        TA0CCR0 = cycles;

        TA0CCTL0 &= ~CAP; // compare mode
        TA0CTL   |= MC0;  // up mode

}

// start up/down mode
void TA0_start_compare_up_down(uint16_t cycles) {

    // disable TAIFG interrupts
        TA0CTL   &= ~TAIE;
    // enable CCIFG interrupts
        TA0CCTL0 |= CCIE; // capture/compare interrupt

    // set timer mode to compare value
        TA0_STOP

    // set value to count in TA0CCR0
        TA0CCR0 = cycles;

        TA0CCTL0 &= ~CAP; // compare mode
        TA0CTL   |= (MC0 | MC1);  // up down mode

}
