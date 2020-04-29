#include <functions/timer/timer.h>
#include <functions/timer_old/timer_A.h>
#include <msp430.h> 
#include <msp430f5xx_6xxgeneric.h>

#include "functions/clock/driverlib_fll.h"
#include "functions/delay/delays.h"
#include "functions/lcd/lcd2x20.h"
#include "configurations/configs.h"

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////

/// Timer Output, used for generating signal to measure
#define PWM_PIN 4
#define PWM_SEL P2SEL
#define PWM_DIR P2DIR

///////////////////////////////////////////////////////

// Labor 5
uint16_t high_low       = 0; // zur Ermittlung der Flankenart
uint16_t first_check    = 0; // Entprellung: erster lesevorgang
uint16_t second_check   = 0; // Entprellung: zweiter lesevorgang
uint8_t  key_pressed    = 0; // Bool

void key_handler(void);

///////////////////////////////////////////////////////

timer* TimerA0;
timer* TimerA1;
timer* TimerA2;
timer* TimerB0;

#define GATE_TIME_OVERFLOW_NUMBER 40
#define FREQ_INPUT_DIR            P1DIR
#define FREQ_INPUT_SEL            P1SEL
#define FREQ_INPUT_PIN            3

uint16_t TA2_overflow_counter = 0;
uint16_t low_high = 1; // gate time flank type
uint16_t z = 0;        // flank counter
uint16_t frequency = 0; // not needed when f=z (1 sec gate time)


/// MAIN
///////////////////////////////////////////////////////

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	/// CLOCK SETTINGS
	///////////////////////////////////////////////////////

	UCS_initFLLSettle(MCLK_SPEED_IN_KHZ, CLK_RATIO);

	UCSCTL5 |=  DIVA_1;        // Divide ACLK by 1
	UCSCTL4 |= (SELA__DCOCLK); // set ACLK source to DCO_DIV


    /// GLOBAL INTERRUPT ENABLE
    ///////////////////////////////////////////////////////

    __bis_SR_register( GIE );


	/// TIMER SETTINGS
	///////////////////////////////////////////////////////

    TimerA0 = malloc(sizeof(timer));
    TimerA1 = malloc(sizeof(timer));
    TimerA2 = malloc(sizeof(timer));
    TimerB0 = malloc(sizeof(timer));

    timer_init( // for frequency calculation
            TimerA0,
            TIMER_A0_BASE,
            TASSEL_1,       // ACLK
            ACLK_SPEED_IN_KHZ,
            ID__1,
            1
    );

    timer_init( // signal to measure
            TimerA2,
            TIMER_A2_BASE,
            TASSEL_1,
            ACLK_SPEED_IN_KHZ,
            ID__1,
            1
    );

    timer_init( // for delays
            TimerA1,
            TIMER_A1_BASE,
            TASSEL_1,
            ACLK_SPEED_IN_KHZ,
            ID__1,
            1
    );


    /// PORT CONFIGURATION
    ///////////////////////////////////////////////////////

    /// LABOR 5
    ///////////////////////////////////////////////////////

	/// Encoder config
    /// setup P2.0 (GS) as interrupt
    GS_DIR &= ~(1<<GS); // input direction on P2.GS
    GS_SEL &= ~(1<<GS); // SEL 0s
    GS_IE  |=  (1<<GS); // interrupt enable
    GS_IES |=  (1<<GS); // Edge select h-l

	ENC_DIR &= ~(A0 | A1 | A2); // Input direction
	GS_DIR  &= ~GS;             // Input direction (not needed?)


    /// LABOR 7
    ///////////////////////////////////////////////////////

    /// select Timer/PWM function
    PWM_SEL |= (1<<PWM_PIN);
    PWM_DIR |= (1<<PWM_PIN);


    /// LABOR 8
    ///////////////////////////////////////////////////////

    // externes signal von P2.4 (TA2.1) an P1.3

    /// configure timer input for measurement
    FREQ_INPUT_DIR &= ~(1<<FREQ_INPUT_PIN); // DIR = 0
    FREQ_INPUT_SEL |=  (1<<FREQ_INPUT_PIN); // SEL = 1

    /// configure TA0.2 Capture mode
    TA0CCTL2 |=  CM_1 + CAP + CCIS_0 + CCIE;   // capture on rising edge + capture mode + CCI2A (P1.3)

    /// gate time timer B0
    TB0CTL |= TBIE + TASSEL_1 + ID__8;
    TB0CTL &= ~(MC0 | MC1);
    TB0CCR0 = 50000-1;
    TB0CTL |= MC__UP; // start up mode

    /// generate signal to measure
    pwm_init(TimerA2, 1); // TA2.1
    pwm_set_period_pulsewidth(TimerA2, 3, 1, 1); // 333 Hz
    pwm_start(TimerA2);


    /// LCD
    ///////////////////////////////////////////////////////

    lcd_init();


    /// MAIN LOOP
    ///////////////////////////////////////////////////////

	while(1) {

	    LPM0;

	    key_handler();

	}

}


/// INTERRUPT SERVICE ROUTINES
///////////////////////////////////////////////////////

/// LABOR 8
///////////////////////////////////////////////////////

// capture timer
#pragma vector=TIMER0_A1_VECTOR
__interrupt void capture_event_interrupt(void) {

    if(TA0IV == 0x04) { // TA0CCR2 CCIFG

        z++; // flank counter

    }

}

// gate timer
#pragma vector=TIMER0_B1_VECTOR
__interrupt void gate_time_interrupt(void) {

   if(TB0IV == 0x0E) { // interrupt source: timer overflow

        TA2_overflow_counter++;

        if(TA2_overflow_counter == GATE_TIME_OVERFLOW_NUMBER) {

            // 1 second reached

            if(low_high) { // low_high == 1, steigende flanke

                z = 0;            // reset capture value

                TA0CCTL2 |= CM_1; // start capture

            } else { // low_high == 0, fallende flanke, evaluate capture

                TA0CCTL2 &= ~ CM_0; // stop capture

                frequency = z;      // calculate frequency, z/tgate = z

                //lcd_print(...);

                LED_OUT &=~ (1<<LED_2);

            }

            low_high ^= 1; // switch flank

            TA2_overflow_counter = 0;

        }

        TB0CTL &= ~TAIFG;

   }

}


/// LABOR 5
///////////////////////////////////////////////////////

// Tastendruck
#pragma vector=PORT2_VECTOR
__interrupt void keypress_interrupt(void) { // react on group select of encoder when key is pressed

    GS_IE &= ~(1<<GS); // disable interrupts


    high_low = (GS_IES & (1<<GS)); // determine flank type, based on that decide path


    if(high_low) { // key pushed down

        first_check = (ENC_IN & ENC_MASK); // read encoder output

        //delay_ms(1); //doesnt work for some reason

        __delay_cycles(MCLK_SPEED_IN_KHZ >> 1); // 500us

        second_check = (ENC_IN & ENC_MASK); // read encoder output again


    } else { // key released

        if(first_check == second_check) {

            key_pressed = 1; // confirm key press

        }

    }

    // toggle flank type that executes interrupt
    // --> a second interrupt will be triggered on button release
    GS_IES ^= (1<<GS);


    // reset IFG
    GS_IFG &= ~(1<<GS);

    // re-enable GS interrupts
    GS_IE |= (1<<GS);


    // return from LPM0
    LPM0_EXIT;

}

///////////////////////////////////////////////////////

void key_handler(void) {

    if(key_pressed) {

        switch(second_check) {

            default:
            case 0: // Taste 0
                lcd_print("Taste 1" ,0,0);
                break;
            case 1: // Taste 1
                lcd_print("..." ,0,0);
                break;
            case 2: // Taste 2
                lcd_print("Taste 2" ,0,0);
                break;
            case 3: // Taste 3
                lcd_print("Taste 3" ,0,0);
                break;
            case 4: // Taste 4
                lcd_print("Taste 4" ,0,0);
                break;
            case 5: // Taste 5

                lcd_print("1 kHz" ,0,0);

                break;
            case 6: // Taste 5

                lcd_print("3 kHz" ,0,0);

                break;
            case 7: // Taste 7

                lcd_print("5 kHz" ,0,0);

                break;
        }

        key_pressed = 0;
    }
}

