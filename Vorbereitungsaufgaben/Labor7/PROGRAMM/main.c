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

/// Timer Output

#define PWM_PIN 4
#define PWM_SEL P1SEL

///////////////////////////////////////////////////////

uint16_t high_low       = 0; // zur Ermittlung der Flankenart
uint16_t first_check    = 0; // Entprellung: erster lesevorgang
uint16_t second_check   = 0; // Entprellung: zweiter lesevorgang
uint8_t  key_pressed    = 0; // Bool


///////////////////////////////////////////////////////

timer* TimerA0;
timer* TimerA1;

void key_handler(void);

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

    timer_init(
            TimerA0,
            TIMER_A0_BASE,
            TASSEL_1,       // ACLK
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


    /// PWM SETTINGS
    ///////////////////////////////////////////////////////

	pwm_init(TimerA0, 3);

	pwm_set_period_pulsewidth(TimerA0, 0, 0, 0);

	pwm_start(TimerA0);

    // select Timer/PWM function
	PWM_SEL |= (1<<PWM_PIN);


    /// PORT CONFIGURATION
    ///////////////////////////////////////////////////////

	/// Encoder config
    /// setup P2.0 (GS) as interrupt
    GS_DIR &= ~(1<<GS); // input direction on P2.GS
    GS_SEL &= ~(1<<GS); // SEL 0s
    GS_IE  |=  (1<<GS); // interrupt enable
    GS_IES |=  (1<<GS); // Edge select h-l

	ENC_DIR &= ~(A0 | A1 | A2); // Input direction
	GS_DIR  &= ~GS;             // Input direction (not needed?)


    /*/// LED settings
    LED_DIR = 0xFF;
    LED_OUT = 0x00;
    LED_OUT &= ~((1<<LED_0) | (1<<LED_1) | (1<<LED_2) );


    /// temporary for testing
    P7DIR |= BIT4;
    P7OUT &= ~BIT4;*/

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
                pwm_set_period_pulsewidth(TimerA0, 1000, 500, 0); // 1kHz

                break;
            case 6: // Taste 5

                lcd_print("3 kHz" ,0,0);
                pwm_set_period_pulsewidth(TimerA0, 333, 166, 0); // 3kHz

                break;
            case 7: // Taste 7

                lcd_print("5 kHz" ,0,0);
                pwm_set_period_pulsewidth(TimerA0, 200, 100, 0); // 5kHz

                break;
        }

        key_pressed = 0;
    }
}

