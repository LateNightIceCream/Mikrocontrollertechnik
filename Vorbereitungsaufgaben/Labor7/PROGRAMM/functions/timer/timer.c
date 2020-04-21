/*
 * imer_pwm.c
 *
 *  Created on: Apr 20, 2020
 *      Author: R. Grünert
 */


#include <functions/timer/timer.h>

void timer_init(
        timer* timer,
        uint16_t   _timer_base_address,
        uint16_t   timer_clock_source,
        uint16_t   clock_source_speed_khz,
        uint16_t   _clock_divider_ID_bits,
        uint16_t   _clock_divider_ID_value
)
{

    timer->source_clockspeed_khz  = clock_source_speed_khz;
    timer->clock_divider_ID_bits  = _clock_divider_ID_bits;
    timer->clock_divider_ID_value = _clock_divider_ID_value;

    //// ADDRESSES
    timer->timer_base_address = (uint16_t* )(_timer_base_address);
    timer->CTL                = (uint16_t* )(_timer_base_address);
    timer->CCTL0              = (uint16_t* )(_timer_base_address + 0x02);

    timer->CCR0               = (uint16_t* )(_timer_base_address + 0x12);

    /// TIMER INITIALISATION

    // TIMER CLOCK SOURCE
        // reset any previous setting
        *(timer->CTL) &= ~(TASSEL_3);
        // write new setting
        *(timer->CTL) |= timer_clock_source;

    // CLOCK DIVIDERS
        // set timer clock dividers
        *(timer->CTL) &= ~(ID_3);
        *(timer->CTL) |= _clock_divider_ID_bits;

        // clear TAR / reset clock divider logic
        *(timer->CTL) |= TACLR;

    // enable TA Interrupts (optional)
        *(timer->CTL) |= TAIE;


}

///////////////////////////////////////////////////////

/// Timer initialization
/// timer:  pointer to timer structure
/// CC_reg: integer value 0,1,2.. to select CCR to use for PWM

void pwm_init( timer* timer, uint16_t CC_register_select) {

    /// initialize timer addresses
    timer->pwm.CCTLn     = ( timer->CTL + 1 + CC_register_select);
    timer->pwm.CCRn      = ( timer->timer_base_address + 9 + CC_register_select);

    timer->pwm.period    = 12; // not really needed
    timer->pwm.pulsew    = 100; // not really needed

    /// disable CCIFG Interrupts
    *(timer->CCTL0)     &= ~(CCIE);
    *(timer->pwm.CCTLn) &= ~(CCIE);

    /// disable timer Interrupts
    *(timer->CTL) &= ~TAIE;

}


///////////////////////////////////////////////////////

// max possible time is 2^16/timer_clock
void pwm_set_period_pulsewidth(timer* timer, uint16_t period, uint16_t pulsewidth, uint8_t us_ms_s) {

    if(pulsewidth <= period) {

        uint16_t period_cycles = 0; // timer count compare value to store inside CCR0
        uint16_t pulsew_cycles = 0; // timer count compare value to store inside CCRn

        uint16_t clockfactor   = (timer->source_clockspeed_khz) / (timer->clock_divider_ID_value);

        switch(us_ms_s) {

            case 0: // micro seconds (may "underflow")
                clockfactor   /= 1000;
                break;

            case 1: // milli seconds
                // khz*ms = s
                // clockfactor /= 1;
                break;

            case 2: // seconds (may overflow)
                clockfactor   *= 1000; // does not work rn
                break;

            default:
                clockfactor = 0;
                break;
        }

        period_cycles = clockfactor * period     - 1;
        pulsew_cycles = clockfactor * pulsewidth - 1;

        if(period_cycles <= 65536) {

            // set period cycles in CCR0
            *(timer->CCR0) = period_cycles;

            // set pulse width cycles in CCR2
            *(timer->pwm.CCRn) = pulsew_cycles;
        }
    }

}

///////////////////////////////////////////////////////

void pwm_start(timer* timer) {

    // start up mode
    *(timer->CTL) &= ~(MC0 | MC1);
    *(timer->CTL) |=   MC0; // up mode

    // set OUTMOD to reset/set
    //*(timer->CCTLn) &= ~(OUTMOD0 | OUTMOD1 | OUTMOD2);
    *(timer->pwm.CCTLn) |=  (OUTMOD0 | OUTMOD1 | OUTMOD2); // reset/set mode

}


///////////////////////////////////////////////////////

void pwm_stop(timer* timer) {

    // stop by setting OUTMOD to 0 and OUT bit to 0 (does not stop the timer counting)
    *(timer->pwm.CCTLn) &= ~(OUTMOD0 | OUTMOD1 | OUTMOD2);
    *(timer->pwm.CCTLn) &= ~(OUT);

}

