
#include <htc.h>
#include <stdint.h>
#include <stdio.h>
#include "config.h"
#include "main.h"
#include "buttons.h"
#include "lcddriver.h"
#include "delay.h"

uint8_t but;

unsigned char WELCOME[] = "PD873 VER %i.%i";

s_setting_datas settings = {9600, 0, 625};
s_analog_datas analogs;

static char DPBUFFER[20];
unsigned int next_ad_data;

unsigned int MAIN_STATE = MAIN_DISPLAY;

/* display refresh counter to display refresh time calculating.. */
static unsigned int display_refresh_counter = 0;

/* A/D average counters, and datas.*/
unsigned long int AD_SUMMA = 0;
unsigned int AD_OVERSAMPLE_COUNTER = 0;
unsigned int AD_VALUE;

s_status PROGRAM_STATUS;    // Program status. and flags.
int8_t ACTUAL_MENU = 0;

#define MAX_MENU_ITEM 2

struct {
  uint8_t feature;
  char * title;
} menu_items[MAX_MENU_ITEM] = {{ 0b00000011, (char*)"INPUT"},{0b00000011, (char*)"TYPE"}};


void interrupt isr(void)
{
  if (PIR1bits.TMR2IF)
  {
    PIR1bits.TMR2IF = 0;
    if (display_refresh_counter++ >= settings.display_refresh_time)
    {
      display_refresh_counter = 0;
      PROGRAM_STATUS.DISPLAY_REFRESH = 1; /*    */
//      LATBbits.LATB4 = !PORTBbits.RB4;
    }
  };
  if (INTCONbits.TMR0IF)
  {
    INTCONbits.TMR0IF = 0;
  }
  if (PIR1bits.ADIF)
  {
    PIR1bits.ADIF = 0;
    next_ad_data = (ADRESL + (ADRESH * 256));
    next_ad_data >>= 6;
    AD_SUMMA += next_ad_data;
    if (AD_OVERSAMPLE_COUNTER++ >= 1024)
    {
      AD_VALUE = AD_SUMMA >> 5;
      AD_SUMMA = 0;
      PROGRAM_STATUS.AD_REFRESH = 1;
      AD_OVERSAMPLE_COUNTER = 0;
//      LED = !LED;
    }
    ADCON0bits.GO = 1;  /* Restart A/D conversion. */
  }
};

void main()
{
	//OPTION 	= 0x80;		// PORTB pull-ups are disabled
  ANSELH = 0;
  TRISBbits.TRISB5 = 0;
  TRISBbits.TRISB4 = 0;

  LED = 1;
  TRISC &= 0b11001111;

/*  T2CONbits.TOUTPS = 0b1111;
  T2CONbits.T2CKPS = 0b10;*/
  T2CON = 0b01111011;
  PR2 = TMR2DATA;
  PIE1bits.TMR2IE = 1;
  IPR1bits.TMR2IP = 1;
  T2CONbits.TMR2ON = 1;

  InitButtons();

  InitLCD();
	LCDSendCmd(DISP_ON);
	LCDSendCmd(CLR_DISP);
  sprintf(DPBUFFER, WELCOME, VERH, VERL);
	LCDSendStr(DPBUFFER);

	TRISAbits.TRISA1 = 0;	// RELAY pin as output
  TRISAbits.TRISA4 = 0; // freq pin as output.
  RELAY = 0;

  ADCON1 = 0;
  ADCON2 = 0b10111110;
  ADCON0 = 0b00000001;

  PIR1bits.ADIF = 0;
  PIE1bits.ADIE = 1;  // Enable A/D interrupt
  INTCONbits.PEIE = 1;
  INTCONbits.GIE = 1;
  ADCON0bits.GO = 1;  // First start A/D conversion.

  while (1)
  {

    but = ButtonScan();

/*    if (but == BUT_UP_OFF) LATBbits.LATB4 = ON;
    else if (but == BUT_DN_OFF) LATBbits.LATB4 = OFF;
    else
    {

    }*/

    DelayMs(200);

    LATBbits.LATB4 = ON;

    DelayMs(200);

    LATBbits.LATB4 = OFF;

  }
  

}
