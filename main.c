
#include <htc.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "main.h"
#include "buttons.h"
#include "lcddriver.h"
#include "delay.h"
#include "menu.c"

uint8_t but;

unsigned char WELCOME[] = "PD18F VER %i.%i";

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

int8_t MENU_LEVEL_01 = 0;   // Position of first menu level.
int8_t MENU_LEVEL_02 = 0;   // Position of second menu level.
int8_t HIGH_LEVEL_MENU;     // Whether where was in main menu ?

void interrupt isr(void)
{
  if (PIR1bits.TMR2IF)
  {
    PIR1bits.TMR2IF = 0;
    if (display_refresh_counter++ >= settings.display_refresh_time)
    {
      display_refresh_counter = 0;
//      PROGRAM_STATUS.DISPLAY_REFRESH = 1; /*    */
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

  PORTA = 0;
  LATA = 0;
  ANSEL = 0x01;
  TRISA = 0x01;     //

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

//  InitButtons();

  InitLCD();

	TRISAbits.TRISA1 = 0;	// RELAY pin as output
  TRISAbits.TRISA4 = 0; // freq pin as output.
  RELAY = 0;

  ADCON1 = 0;
  ADCON2 = 0b10111101;
  ADCON0 = 0b00000001;

  PIR1bits.ADIF = 0;
  PIE1bits.ADIE = 1;  // Enable A/D interrupt
  INTCONbits.PEIE = 1;
  INTCONbits.GIE = 1;
  ADCON0bits.GO = 1;  // First start A/D conversion.
  PROGRAM_STATUS.MUST_REDRAW = 1;

  while (1)
  {

    but = ButtonScan();

    switch (MAIN_STATE)
    {

/* ------------------------- MAIN DISPLAY ----------------------------------*/
    case MAIN_DISPLAY:
      if (PROGRAM_STATUS.MUST_REDRAW)
      {
        LCDSendCmd(DISP_ON);
        LCDSendCmd(CLR_DISP);
        sprintf(DPBUFFER, WELCOME, VERH, VERL);
        LCDSendStr(DPBUFFER);
        PROGRAM_STATUS.MUST_REDRAW = 0;
      }
    if (PROGRAM_STATUS.AD_REFRESH)
    {
      sprintf(DPBUFFER, "%6i", AD_VALUE);
      LCDSendCmd(DD_RAM_ADDR2);
      LCDSendStr(DPBUFFER);
      PROGRAM_STATUS.DISPLAY_REFRESH = 0;
      PROGRAM_STATUS.AD_REFRESH = 0;
    }

/* ------------------------- MAIN DISPLAY BUTTONS HANDLE --------------------*/

    switch (but)
    {
      case BUT_OK_OFF:
        MAIN_STATE = MENU_DISPLAY;
        PROGRAM_STATUS.MUST_REDRAW = 1;
        break;
      case BUT_RG_OFF:
        break;
    }


/* ------------------------ MAIN DISPLAY END -------------------------------*/
        break;

/* ------------------------ MENU DISPLAY -----------------------------------*/

    case MENU_DISPLAY:

      if (PROGRAM_STATUS.MUST_REDRAW)
      {
        LCDSendCmd(CLR_DISP);
        LCDSendCmd(DD_RAM_ADDR + ((DISPLAY_WIDTH - strlen(main_menus[ACTUAL_MENU].title)) / 2));
        sprintf(DPBUFFER, "%s", main_menus[ACTUAL_MENU].title);
        LCDSendStr(DPBUFFER);
        PROGRAM_STATUS.MUST_REDRAW = 0;
      }

/* ------------------------ MENU DISPLAY BUTTONS HANDLE ---------------------*/

    switch (but)
    {
      case BUT_UP_OFF:
        if (ACTUAL_MENU == (MAIN_MENU_ITEMS - 1)) { ACTUAL_MENU = 0; }
        else ACTUAL_MENU++;
        PROGRAM_STATUS.MUST_REDRAW = 1;
        break;
      case BUT_DN_OFF:
        if (ACTUAL_MENU == 0){ ACTUAL_MENU = (MAIN_MENU_ITEMS - 1); }
        else {ACTUAL_MENU--;}
        PROGRAM_STATUS.MUST_REDRAW = 1;
        break;
        /* Enter the submenu, if there are.*/
      case BUT_OK_OFF:
        /* Save the high level menu.*/
        HIGH_LEVEL_MENU = ACTUAL_MENU;
        ACTUAL_MENU = 0;
        
        break;
        /* Back the menu level, if there, or MAIN display.*/
      case BUT_ES_OFF:
        MAIN_STATE = MAIN_DISPLAY;
        PROGRAM_STATUS.MUST_REDRAW = 1;
        break;
    }

        break;
    }


/*    if (but == BUT_UP_OFF) LATBbits.LATB4 = ON;
    else if (but == BUT_DN_OFF) LATBbits.LATB4 = OFF;
    else
    {

    }*/


  }
  

}