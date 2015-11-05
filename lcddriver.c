
#include <htc.h>
#include "lcddriver.h"
#include "delay.h"

unsigned  int i;
unsigned  char data;

void E_Pulse(void) {
	E=1;
	asm("nop");
	asm("nop");
	asm("nop");
	E=0;
}

// Init LCD  after reset
void InitLCD(void) {

	INTCON 	= 0x0;			// Disable inerupt
  ANSEL = 0x0;
//	ADCON1  = 0x06;			// Port as Digital IO
  
  TRISAbits.TRISA2 = 0; // TRISA2 	= 0;			// RS pin as output
	TRISAbits.TRISA3 = 0; // TRISA3 	= 0;			// RW pin as output
	TRISAbits.TRISA5 = 0; // TRISA5 	= 0;			// E pin as output
	TRISC	&= 0xF0;			// D4-D7 as output


	RS=0;
	RW=0;
	DelayMs(110);
	PORTC=0b00000011;
	E_Pulse();
	DelayMs(10);
	PORTC=0b00000011;
	E_Pulse();
	DelayMs(10);
	PORTC=0b00000011;
	E_Pulse();
	DelayMs(10);
	PORTC=0b00000010;
	E_Pulse();
}

// Send char to LCD
LCDSendChar(unsigned char c) {

	DelayMs(2);
	//get upper nibble
	data = c & 0b11110000;
	//set D4-D7
	data = data >> 4;
	//send data to LCD
	PORTC = data;
	//set LCD to write
	RW=0;
	//set LCD to data mode
	RS=1;
	//toggle E for LCD
	E_Pulse();
	// get lower nibble
	data = c & 0b00001111;
	//send data to LCD
	PORTC = data;
	//set LCD to write
	RW=0;
	//set LCD to data mode
	RS=1;
	//toggle E for LCD
	E_Pulse();
}

// Send command to LCD
void LCDSendCmd(unsigned char c) {

	DelayMs(2);
	//get upper nibble
	data = c & 0b11110000;
	//set D4-D7
	data = data >> 4;
	//send data to LCD
	PORTC = data;
	//set LCD to write
	RW=0;
	//set LCD to data mode
	RS=0;
	//toggle E for LCD
	E_Pulse();
	// get lower nibble
	data = c & 0b00001111;
	PORTC = data;
	//set LCD to write
	RW=0;
	//set LCD to data mode
	RS=0;
	//toggle E for LCD
	E_Pulse();
}

void LCDSendStr(unsigned char* str) {

	i=0;
	while(str[i]) {
		LCDSendChar(str[i]);
		i++;
	}
}

