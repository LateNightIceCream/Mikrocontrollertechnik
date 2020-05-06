//***************************************************************************
//Datum: 10.04.2020  ;Autor:Gruppe4
//Projektname:Lab8  ;Programmname: lab8_appl4.c
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
unsigned char Func_REC_Flag; //Func_REC=1 aktiv
unsigned char Func_FX_Flag;//FX_Flag=1 wenn Torzeit erreicht ist
unsigned long int z; //gezählte Flanken
unsigned int n; //Timerüberlaufe
unsigned long int frequenz; //berechnete Frequenz
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
//Lab 8
#pragma vector=TIMERA0_VECTOR
__interrupt void TIMERA0_ISR(void)
{
	TACCTL0 &= ~CCIE; //local enable off
	z++;
	//TAIFG &=0b00000000; //Interrupt flag reset
	TACCTL0 |= CCIE; //local enable on
}
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERB0_ISR(void)
{
	TBCCTL0 &= ~CCIE;
	n++;
	if (n==400)
	{
	   frequenz=z; //Frequenz = Anzahl Timerüberläufe/1s in Hz
	   TaF1LED_TOGGLE;
	   Func_FX_Flag=1;
	   n=0;
	   z=0;
	}
	TBCCTL0 |= CCIE;
}
/****************************************************************************/
void main(void)
{
  unsigned char TF_sel;

  WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
  clock_init();// Funktion Taktinitialisierung
  port_init();//Ports initialisieren in system.c
  lcd_init();//lcd initialisieren

  lcd_printf("MCS2618II Lab8",0,0);
  lcd_printf("24.04.2020",1,0);
  TaF1LED_TOGGLE;
  delay_ms(2000);//Ausschrift
  LCD_CLS; //LCD loeschen (Cursor nicht sichtbar)
  LCD_HOME;//Cursor Home (0,0)
  Key8Flag=0;
  TF_sel=0;
  Func_REC_Flag=0;
  Func_FX_Flag=0;
  TaF1LED_OFF;

  P2DIR  = 0b11111011;
  P2SEL |= 0b00010100; //P2.4 auf Peripheriefunktion
  P2OUT  = 0b11111111;

  //f=1kHz 1:2
  //TACTL   = TACLR + MC_0; //stop,
  //TACCTL2 = OUTMOD_4;
  //TACCR0  = 8000-1;
  //TACCR2  = 4000;

  //Lab8
  z=0; //gezählte Flanken
  n=0; //Timerüberläufe

  //TimerA Initialisierung
  //TACTL = TASSEL_1 + TACLR; //ACLK, Clear Timer A Register
  TACCTL0 = CM_0 //No Capture
  + CCIS_1 		 //CCI0B Eingang P2.2
  + CAP			 //Capture Mode
  + CCIE; 		 //Capture/CompareInterrupt Enable

  // TimerB Initialisierung
  TBCTL = TBSSEL_1 + ID_3 + TBCLR + MC_0; //ACLK, Vorteiler 8, Clear TimerB Register, Stop
  TBCCTL0 = CCIE;
  TBCCR0  = 50000-1;//25ms ium up-mode
  _BIS_SR(GIE);//Globale Interruptfreigabe
  while(1)
  {
   if(Func_FX_Flag==1) //Berechnung Frequenz nach Ablauf der Torzeit
	        {
	          LCD_CLS;
	          lcd_printf("Fx=",1,0);
	          dis_zahl_long(frequenz, 1, 1, 4);
	          Func_FX_Flag=0;
	        }

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

      if (Key8Code==1)
      {
    	  TF_sel++;
    	  Func_REC_Flag=1;
      }

      if (TF_sel==7)
      	 TF_sel=0;

      switch (TF_sel)
      {
        case 0:TACTL = TACLR + MC_0; Func_REC_Flag=0; lcd_printf("TA stop",1,0);break;
        case 1:TACCR0  = 16000-1;TACCR2  = 8000; lcd_printf("sel1",1,12);break;//1kHz 1:2
        case 2:TACCR0  = 16000-1;TACCR2  = 4000; lcd_printf("sel2",1,12);break;//1kHz 1:4
        case 3:TACCR0  = 8000-1;TACCR2  =  4000; lcd_printf("sel3",1,12);break;//2kHz 1:2
        case 4:TACCR0  = 8000-1;TACCR2  =  2000; lcd_printf("sel4",1,12);break;//2kHz 1:4
        case 5:TACCR0  = 2000-1;TACCR2  =  1000; lcd_printf("sel5",1,12);break;//8kHz 1:2
        case 6:TACCR0  = 2000-1;TACCR2  =  500; lcd_printf("sel6",1,12);break;//8kHz 1:4
        default: TACCR0  = 500-1;TACCR2  = 100; //32kHz 1:5
      }

     if(Func_REC_Flag)
     {
       TACTL   = TACLR + TASSEL_1 + MC_1 + ID_0; //ACLK, up/down,
       TACCTL2 = OUTMOD_7;
     }

      //delay_ms(1000);
      //LCD_CLS;
      Key8Flag=0;

      //Lab8
      if(Key8Code==2) //Start Frequenzmessung
      {
    	  TACCTL0 |= CM_1; //CM_1 Capture auf steig. Flanke
    	  //TBCTL = TBSSEL_1 + ID_3 + TBCLR + MC_1; //MC Up Mode
    	  TBCTL |= MC_1; //MC Up Mode
      }



      if(Key8Code==3) //Reset Frequenzmessung
      {
    	  TACCTL0 = CM_0 + CCIS_1 + CAP + CCIE;
    	  TBCTL = TBSSEL_1 + ID_3 + TBCLR + MC_0;
    	  z=0;
    	  n=0;
    	  TBCCR0=0;
      }

    }
  }//end while(1)
}//end main


