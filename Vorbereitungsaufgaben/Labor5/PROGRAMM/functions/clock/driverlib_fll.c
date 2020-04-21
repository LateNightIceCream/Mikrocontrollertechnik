/*
 * driverlib_fll.c
 *
 *  Created on: Apr 6, 2020
 *  From driverlib ucs.c
 */

#include "driverlib_fll.h"

void UCS_initFLLSettle(uint16_t fsystem, uint16_t ratio) {

    volatile uint16_t x = ratio * 32; // calculate number of cycles/30 to delay

    // init FLL
    UCS_initFLL(fsystem, ratio);

    // delay cycles to let clock settle (see user's guide msp430x5xx 5.2.7 FLL)

    while(x--) {

       __delay_cycles(30);

    }
}

void UCS_initFLL(uint16_t fsystem, uint16_t ratio) {

    uint16_t d, dco_div_bits;
    uint16_t mode = 0;

    //Save actual state of FLL loop control, then disable it. This is needed to
    //prevent the FLL from acting as we are making fundamental modifications to
    //the clock setup.
    uint16_t srRegisterState = __get_SR_register() & SCG0;

    d = ratio;
    dco_div_bits = FLLD__2; // FLLD = 1

    if(fsystem > 16000) { // 16 MHz

        d >>= 1; // >> 1 : divide by 2
        mode = 1;

    } else {
        fsystem <<= 1; // multiply by 2
    }

    while(d > 512) {

        // Set next higher div level
        dco_div_bits = dco_div_bits + FLLD0;
        d >>= 1;

    }

    // Disable FLL (see figure 5-1 in user's guide and 6.3.3 Status Register)
    __bis_SR_register(SCG0); // set Status Register Bits


    // FREQUENCY RANGE
    // set DCO to lowest Tap
    UCSCTL0_H = 0x0000; // _H High byte = 0; 16bit value for some reason but whatever

    // set FLLN bits to zero
    UCSCTL2 &= ~(0x03FF);

    UCSCTL2 = dco_div_bits | (d-1);

    if(fsystem <= 640) { // fsystem < 0.63 MHz
        UCSCTL1 = DCORSEL_0;
    } else if(fsystem < 1250) { // fsystem < 1.25 MHz
        UCSCTL1 = DCORSEL_1;
    } else if(fsystem < 2500) { // 1.25 MHz < fsystem < 2.5 MHz
        UCSCTL1 = DCORSEL_2;
    } else if(fsystem < 5000) { // 2.5 MHz < fsystem < 5 MHz
        UCSCTL1 = DCORSEL_3;
    } else if(fsystem < 10000) { // 5 MHz < fsystem < 10 MHz
        UCSCTL1 = DCORSEL_4;
    } else if(fsystem < 20000) { // 10 MHz < fsystem < 20 MHz
        UCSCTL1 = DCORSEL_5;
    } else if(fsystem < 40000) { // 20 MHz < fsystem < 40 MHz
        UCSCTL1 = DCORSEL_6;
    } else {
        UCSCTL1 = DCORSEL_7;
    }


    // Re-enable FLL
    __bic_SR_register(SCG0);

    // Fault flags
    while(UCSCTL7_L & DCOFFG) {

        // Clear OSC fault flags
        UCSCTL7_L &= ~(DCOFFG);

        // Clear OFIFG fault flag
        SFRIFG1 &= ~(OFIFG);

    }


    // Restore previous SCG0 (Clear Status Register Bits)
    __bis_SR_register(srRegisterState);

    if(mode == 1) { // fsystem > 16 MHz

        // Select DCOCLK
        UCSCTL4 &= ~(SELM_7 + SELS_7); // select main clock source, smclk

        UCSCTL4 |= SELM__DCOCLK + SELS__DCOCLK;

    } else {

        // Select DCODIVCLK
        UCSCTL4 &= ~(SELM_7 + SELS_7);
        UCSCTL4 |= SELM__DCOCLKDIV + SELS__DCOCLKDIV;

    }

}



