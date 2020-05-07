//***************************************************************************
//Datum: 06.05.2020  ;Autor:RG, Gruppe5
//Projektname: Lab9  ;Programmname: lab8_appl.c
//Hardware: MCS2618R4+MCS2618II_BP1
//Programmfunktion: Labor MCT2020
//Bemerkungen:VCC=3,3V, fclk=16MHz
//****************************************************************************

#include  <msp430.h>
#include "system.h"
#include "config_mcs2618r4.h"
#include "lcd2x16.h"
#include "math.h"
/****************************************************************************/
//Funktionsdeklarationen


/****************************************************************************/
//Deklaration aller globalen Variablen

#define TA_CLK_KHZ 2000
#define TA_OVERFLOW_FACTOR 65535/TA_CLK_KHZ

unsigned int overflow_counter = 0; // overflow zähler
unsigned int capture1 = 0; // zur Speicherung der Capturewerte
unsigned int capture2 = 0;
unsigned char low_high;

unsigned float T=0;

void measure_period(void);
void measure_pulsewidth(void);
void measure_pausendauer(void);

/****************************************************************************/
#pragma vector=TIMERA1_VECTOR
__interrupt void Timera1_ISR(void)
{
  if(TAIV == 0x0E) {   // TAIFG
    overflow_counter++;// overflow zähler
  }
}

#pragma vector=TIMERA01_VECTOR
__interrupt void Timera0_ISR(void) {// capture interrupt

  measure_period();
  // measure_pulsewidth();
  // measure_pausendauer();

  // reset interrupt flag
  TAIFG = 0;

  LPM0_EXIT;

}

/****************************************************************************/
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
  clock_init();// Funktion Taktinitialisierung
  port_init();//Ports initialisieren in system.c
  lcd_init();//lcd initialisieren

  lcd_printf("MCS2618II Lab9",0,0);
  lcd_printf("07.05.2020",1,0);
  TaF1LED_TOGGLE;
  delay_ms(2000);//Ausschrift

  LCD_CLS; //LCD loeschen (Cursor nicht sichtbar)
  LCD_HOME;//Cursor Home (0,0)

  TaF1LED_OFF;

  P2SEL = 0b00010100; //P2.2 Alternativfunktion
  P2DIR = 0b11111011; //

  TACTL |=   TACLR
           + TASSEL_1
           + ID_3
           + MC_2 // continuous mode
           + TAIE;

  low_high = 1;

  TACCTL0 |= CCIS_1 + CCIE + CAP;

  TACCTL0 |= CM_1; // capture on rising edge
  //  TACCTL0 |= CM_2; // capture on falling edge

  _BIS_SR(GIE);//Globale Interruptfreigabe

  lcd_printf("fx=            Hz",1,0);

  while(1)
  {
    _nop();

    LPM0;

    // calculate time
    T = TA_OVERFLOW_FACTOR * overflow_counter + abs(capture2 - capture1 - 1)/TA_CLK_KHZ; // Einheit: ms

    // Messwert ausgeben




  }//end while(1)
}//end main


void measure_period() {

  if(low_high) { // es ist in beiden Fällen ein low_high, aber ich verwende die gleiche Variable

    // erste low_high Flanke

    capture1 = TACCR0;

    //capture mode wird nicht umgeschalten

  } else {

    capture2 = TACCR0;
    //capture mode wird nicht umgeschalten

  }

  low_high ^= 1;

}

void measure_pulsewidth() {

  // pulse width measurement
  if(low_high){

    capture1 = TACCR0;

    // capture mode high->low
    TACCTL0 &= ~(CM_3);
    TACCTL0 |= CM_2; // switch zu high_low flanke

  } else {

    capture2 = TACCR0;

    // capture mode low->high
    TACCTL0 &= ~(CM_3);
    TACCTL0 |= CM_1;

  }

  low_high ^= 1;

}

void measure_pausendauer() {

  // pulse width measurement
  if(low_high){

    // get timer value
    capture1 = TACCR0;

    // capture mode low->high
    TACCTL0 &= ~(CM_3);
    TACCTL0 |= CM_1; // switch zu low_high flanke

  } else {

    // get timer value
    capture2 = TACCR0;

    // capture mode high->low
    TACCTL0 &= ~(CM_3);
    TACCTL0 |= CM_2;

  }

  low_high ^= 1;

}

