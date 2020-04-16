#include <msp430.h> 
#include <msp430f5xx_6xxgeneric.h>

#include "functions/clock/driverlib_fll.h"
#include "functions/timer/timer_A.h"
#include "functions/delay/delays.h"

#include "configurations/configs.h"
#include "system.h"
#include "config_msc2618r4.h"
#include "lcd2x16.h"

/*
#define LED_DIR P1DIR
#define LED_OUT P1OUT

#define LED_0 2
#define LED_1 3
#define LED_2 4

#define LED_OFF (LED_OUT &= ~((1<<LED_0) | (1<<LED_1) | (1<<LED_2)))
// set the 3 leds to x, a 3 bit number
#define LED_SET(n)  (LED_OUT |= ((( n & BIT0) >> 0 ) << LED_0) | ((( n & BIT1) >> 1 ) << LED_1) | ((( n & BIT2) >> 2 ) << LED_2) )
*/

// port 3 encoder output
#define ENC_DIR P3DIR
#define ENC_IN  P3IN
#define A0      0
#define A1      1
#define A2      2
#define ENC_MASK (1<<A0 | 1 << A1 | 1 << A2)

// port 2 encoder group select (for interrupt)
#define GS_DIR  P2DIR
#define GS_IE   P2IE
#define GS_IES  P2IES
#define GS_IFG  P2IFG
#define GS      0       // pin

uint16_t high_low       = 0; // zur Ermittlung der Flankenart
uint16_t first_check    = 0; // Entprellung: erster lesevorgang
uint16_t second_check   = 0; // Entprellung: zweiter lesevorgang
uint8_t  key_pressed    = 0; // Bool

void key_handler(void);

/*
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	// CLOCK SETTINGS
	UCS_initFLLSettle(MCLK_SPEED_IN_KHZ, CLK_RATIO);

	UCSCTL5 |=  DIVA_2;  // Divide ACLK by 2
	UCSCTL4 |= (SELA_4); // set ACLK source to DCO_DIV

	// TIMER SETTINGS
	TA0_init(TA0_CLK_SOURCE, TA0_CLK_DIVIDE_ID, TA0_CLK_DIVIDE_TAIDEX);


    __bis_SR_register( GIE ); // global interrupt enable
    GS_IE |= (1<<GS);         // GS Interrupt enable

	// Interrupt bei high-->low Flanke einreihen
	GS_IES  |= (1<<GS);

	while(1) {

	    LPM0;

	    // key_handler() wird ausgeführt wenn der interrupt LPM0 zurücksetzt
	    key_handler();

	}

}

#pragma vector=PORT2_VECTOR
__interrupt void keypress_interrupt(void) { // react on group select of encoder when key is pressed

    //LCD_CLS; // clear screen

    // Bestimmung der Flankenart, die den Interrupt ausgelöst hat
    high_low = GS_IES & (1<<GS); // 1: high-->low, 0: low-->high

    // Entscheidung je nach Flankenart
    if(high_low) { // high-->low flank am GS bits

        first_check = ENC_IN & ENC_MASK;

        delay_ms(1); // debounce time ?

        second_check   = ENC_IN & ENC_MASK;

    } else { // low --> high flank, taster losgelassen

        if(first_check == second_check) {

           key_pressed = 1;

        }

    }

    // toggle GS_IES, bei Loslassen des Tasters (low-->high Flanke) wird ISR erneut ausgeführt, else-Zweig oben wird betreten
    GS_IES ^=  (1<<GS);

    // reset IFG
    GS_IFG &= ~(1<<GS);

    // SR Register auf Stack so bearbeiten, dass bei RETI der normale Betriebszustand wiederhergestellt wird
    LPM0_EXIT;

}

void key_handler(void) {

    if(key_pressed) {

        switch(second_check) {

            default:
            case 0:
                lcd_printf("Taste 0" ,0,0);
                break;
            case 1:
                lcd_printf("Taste 1" ,0,0);
                break;
            case 2:
                lcd_printf("Taste 2" ,0,0);
                break;
            case 3:
                lcd_printf("Taste 3" ,0,0);
                break;
            case 4:
                lcd_printf("Taste 4" ,0,0);
                break;
            case 5:
                lcd_printf("Taste 5" ,0,0);
                break;
            case 6:
                lcd_printf("Taste 6" ,0,0);
                break;
            case 7:
                lcd_printf("Taste 7" ,0,0);

                break;

        key_pressed = 0;

        }

    }
}

