
#include <htc.h>
#include "buttons.h"
#include "delay.h"


//unsigned char B1, B2, B3, B4, B5, B6;
s_buttons BM;

void InitButtons()
{
  BM.BB = 0;
//	B1=B2=B3=B4=B5=B6=0;
}

char ButtonScan(void) {

	//set COL1 ---------------------------------------------
	PORTB |= 0x07;
	TRISB |= 0x07;
	bitclr(TRISB, 0);
	DelayMs(2);

	//button 1 is pressed
	if((COL2==1)&&(BM.B1==OFF)) { BM.B1=ON; return BUT_UP_ON; }
	if((COL2==0)&&(BM.B1==ON))  { BM.B1=OFF; return BUT_UP_OFF; }

	//button 6 is pressed
	if((COL3==1)&&(BM.B6==OFF)) { BM.B6=ON; return BUT_ES_ON; }
	if((COL3==0)&&(BM.B6==ON))  { BM.B6=OFF; return BUT_ES_OFF; }

	//set COL1 ---------------------------------------------
	PORTB |= 0x07;
	TRISB |= 0x07;
	bitclr(TRISB,1);
	DelayMs(2);

	//button 4 is pressed
	if((COL1==1)&&(BM.B4==OFF)) { BM.B4=ON; return BUT_RG_ON; }
	if((COL1==0)&&(BM.B4==ON))  { BM.B4=OFF; return BUT_RG_OFF; }

	//button 2 is pressed
	if((COL3==1)&&(BM.B2==OFF)) { BM.B2=ON; return BUT_OK_ON; }
	if((COL3==0)&&(BM.B2==ON))  { BM.B2=OFF; return BUT_OK_OFF; }

	//set COL1 ---------------------------------------------
	PORTB |= 0x07;
	TRISB |= 0x03;
	bitclr(TRISB,2);
	DelayMs(2);

	//button 3 is pressed
	if((COL1==1)&&(BM.B3==OFF)) { BM.B3=ON; return BUT_LF_ON; }
	if ((COL1==0)&&(BM.B3==ON)) { BM.B3=OFF; return BUT_LF_OFF; }

	//button 5 is pressed
	if((COL2==1)&&(BM.B5==OFF)) { BM.B5=ON; return BUT_DN_ON; }
	if((COL2==0)&&(BM.B5==ON))  { BM.B5=OFF; return BUT_DN_OFF; }

	return BUTTON_NONE;
}
