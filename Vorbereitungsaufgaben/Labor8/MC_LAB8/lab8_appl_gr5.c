//***************************************************************************
//Datum: 10.04.2020  ;Autor:GR5
//Projektname:Lab8  ;Programmname: lab8_appl.c
//Hardware: MCS2618R4+MCS2618II_BP1
//Programmfunktion: Labor MCT2020
//Bemerkungen:VCC=3,3V, fclk=16MHz
//****************************************************************************
#include  <msp430.h>
#include "system.h" 
#include "config_mcs2618r4.h"
#include "lcd2x16.h"
/****************************************************************************/
//Funktionsdeklarationen


/****************************************************************************/
//Deklaration aller globalen Variablen
unsigned char Key8Code;//Tastencode
unsigned char Key8Flag;//1: gueltige Taste gedrueckt  0:keine Taste gedrueckt
unsigned int z; //fx-zaehler
unsigned int overflow_cou_timB;
/****************************************************************************/
//Definition aller INTERRUPT-SERVICEROUTINEN
#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR(void)
{
  P1IE &=0b11101111; //P1.4 IE disable
  delay_ms(8);

  if ((P1IES &=0b00010000) == 0b00010000) //HL-Flanke=1?
  {
    Key8Code=(P4IN & 0b00000111);
  }

  if ((P1IES &=0b00010000) == 0b00000000) //LH-Flanke=0?
  {
	  Key8Flag=1;
  }
  P1IE  |=0b00010000; //P1.4 IE enable
  P1IFG &=0b11101111; //IFG reset
  P1IES ^=0b00010000; //IES umschalten
}

#pragma vector=TIMERA0_VECTOR
__interrupt void Timera0_ISR(void)
{
   z++;
}

#pragma vector=TIMERB1_VECTOR
__interrupt void Timerb1_ISR(void)
{
	if(TBIV == 0x0E)  //TBIFG
	{

	  overflow_cou_timB++;
	  if (overflow_cou_timB==400)
	  {
		dis_space(1,3,6);
		dis_zahl65535(1,1,4,z);
		z=0;
		TaF1LED_TOGGLE;
		overflow_cou_timB=0;
	  }
	}
}





/****************************************************************************/
void main(void)
{
  WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
  clock_init();// Funktion Taktinitialisierung
  port_init();//Ports initialisieren in system.c
  lcd_init();//lcd initialisieren
  lcd_printf("MCS2618II Lab8",0,0);
  lcd_printf("30.04.2020",1,0);
  TaF1LED_TOGGLE;
  delay_ms(2000);//Ausschrift
  LCD_CLS; //LCD loeschen (Cursor nicht sichtbar)
  LCD_HOME;//Cursor Home (0,0)
  Key8Flag=0;
  TaF1LED_OFF;
  z=0;
  overflow_cou_timB=0;

  P2SEL = 0b00010100; //P2.2 Alternativfunktion
  P2DIR = 0b11111011; //

  TBCCR0 = 50000-1;
  TBCTL = TBCLR + TBSSEL_1 + ID_3 + MC_1 + TBIE;

  TACCTL0 = CCIS_1 + CM_1 + CAP + CCIE;

  _BIS_SR(GIE);//Globale Interruptfreigabe

  lcd_printf("fx=            Hz",1,0);
  while(1)
  {
    _nop();
    //lcd_printf("wait for key",1,0);
    if(Key8Flag==1)
    {
      LCD_CLS;
      switch (Key8Code)
      {
        case 0: lcd_printf("TaF3",0,0); break;
        case 1: lcd_printf("TaF2",0,0); break;
        case 2: lcd_printf("TaF1",0,0); break;
        case 3: lcd_printf("Ta_Enter",0,0); break;
        case 4: lcd_printf("TaF_down",0,0); break;
        case 5: lcd_printf("Ta_up",0,0); break;
        case 6: lcd_printf("Ta_right",0,0); break;
        case 7: lcd_printf("Ta_left",0,0); break;
      }
      //delay_ms(1000);
      //LCD_CLS;
      Key8Flag=0;
    }
  }//end while(1)
}//end main


