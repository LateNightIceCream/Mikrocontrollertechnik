/*
 * timer_A.c
 *
 *  Created on: Apr 6, 2020
 *      Author: R. Grünert
 */

#include <functions/timer_old/timer_A.h>

///////////////////////////////////////////////////////

/// TIMER A0

void TA0_init(uint16_t clocksource, uint16_t ID_divide) {

    // TIMER CLOCK SOURCE
        // reset any previous setting
        TA0CTL &= ~(TASSEL_3);
        // write new setting
        TA0CTL |= clocksource;

    // CLOCK DIVIDERS
        // set timer clock dividers
        TA0CTL &= ~(ID_3);
        TA0CTL |= ID_divide;

        // clear TAR / reset clock divider logic
        TA0CTL |= TACLR;

    // enable TA Interrupts
        TA0CTL |= TAIE;

}

/*void TA0_compare_cont(void) {

    // stop timer
        TA0_STOP

    // set timer mode to compare continuous
        TA0CCTL0 &= ~CAP; // compare mode
        TA0CTL   |=  MC1;  // continuous mode

}


///////////////////////////////////////////////////////

// start up mode
// cycles: Clock cycles to count
// ccrx:   Capture Compare Register of Timer A0
void TA0_compare_up(uint16_t cycles, uint8_t ccrx) {

    // stop timer (reset MC0, MC1)
        TA0_STOP

    // reset counter
        TA0CTL |= TACLR;

    switch(ccrx) { // which CCR to use

    // CCR0
        case 0:
            // set value to count in CCR0
            TA0CCR0 = cycles;
            // compare mode
            TA0CCTL0 &= ~CAP;

        break;

     // CCR1
        case 1:
            // set value to count in CCR1
            TA0CCR1 = cycles;
            // compare mode
            TA0CCTL1 &= ~CAP;

        break;

     // CCR2
        case 2:
            // set value to count in CCR1
            TA0CCR2 = cycles;
            // compare mode
            TA0CCTL2 &= ~CAP;

        break;

     // else
        default:
        break;
    }

    TA0CTL   |= MC0;  // start up mode

}


///////////////////////////////////////////////////////

// start up/down mode, unfinished
void TA0_start_compare_up_down(uint16_t cycles) {

    // stop timer
        TA0_STOP

    // reset counter
        TA0CTL |= TACLR;

    // set timer mode to compare value
        TA0_STOP

    // set value to count in TA0CCR0
        TA0CCR0 = cycles;

        TA0CCTL0 &= ~CAP; // compare mode
        TA0CCTL0 &= ~(CM0 | CM1); // compare mode
        TA0CTL   |= (MC0 | MC1);  // up down mode

}
*/

///////////////////////////////////////////////////////

/// TIMER A1

void TA1_init(uint16_t clocksource, uint16_t ID_divide) {

    // TIMER CLOCK SOURCE
        // reset any previous setting
        TA1CTL &= ~(TASSEL_3);
        // write new setting
        TA1CTL |= clocksource;

    // CLOCK DIVIDERS
        // set timer clock dividers
        TA1CTL &= ~(ID_3);
        TA1CTL |= ID_divide;

        // clear TAR / reset clock divider logic
        TA1CTL |= TACLR;

    // enable TA Interrupts
        TA1CTL |= TAIE;

}

///////////////////////////////////////////////////////
