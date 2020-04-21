/*
 * timer_pwm.h
 *
 *  Created on: Apr 20, 2020
 *      Author: R. Grünert
 */

#ifndef FUNCTIONS_TIMER_TIMER_H_
#define FUNCTIONS_TIMER_TIMER_H_

#include <functions/timer_old/timer_A.h>
#include <msp430.h>
#include <stdint.h>

///////////////////////////////////////////////////////




typedef struct timer_struct {

    // basic properties
    uint16_t clock_source;
    uint16_t source_clockspeed_khz;
    uint16_t clock_divider_ID_bits;
    uint16_t clock_divider_ID_value;

    // timer registers
    uint16_t* timer_base_address;
    uint16_t* CTL;                // control register address
    uint16_t* CCR0;
    uint16_t* CCTL0;


    struct timer_pwm_struct {

        // PWM Settings
        uint16_t* CCTLn;    // capture compare control register address
        uint16_t* CCRn;     // the CCR register address where PWM is set up

        uint16_t period;    // timer period time value
        uint16_t pulsew;    // timer pulse width time value

    } pwm;

} timer;



///////////////////////////////////////////////////////

void timer_init(
        timer* timer,
        uint16_t   _timer_base_address,
        uint16_t   timer_clock_source,
        uint16_t   clock_source_speed,
        uint16_t   _clock_divider_ID_bits,
        uint16_t   _clock_divider_ID_value
);


////////////////////////////////////////////s///////////

void pwm_init( timer* timer, uint16_t CC_register_select);


///////////////////////////////////////////////////////

void pwm_set_period_pulsewidth(timer* timer, uint16_t period, uint16_t pulsewidth, uint8_t us_ms_s);
void pwm_start(timer* timer);
void pwm_stop(timer* timer);

#endif /* FUNCTIONS_TIMER_TIMER_H_ */
