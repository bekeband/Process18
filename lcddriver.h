/* 
 * File:   lcddriver.h
 * Author: user
 *
 * Created on 2015. október 9., 13:37
 */

#ifndef LCDDRIVER_H
#define	LCDDRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DISPLAY_WIDTH   16

#define		E       LATAbits.LA5 // RA5
#define		RW			LATAbits.LA3 //RA3
#define		RS			LATAbits.LA2 // RA2

#define		CLR_DISP		0x01
#define		DISP_ON			0x0C
#define		DISP_OFF		0x08
#define		CUR_HOME        0x02
#define		CUR_OFF 		0x0C
#define 	CUR_ON_UNDER    0x0E
#define		CUR_ON_BLINK    0x0F
#define 	CUR_LEFT        0x10
#define		CUR_RIGHT       0x14
#define		CUR_UP  		0x80
#define		CUR_DOWN		0xC0
#define		ENTER           0xC0
#define		DD_RAM_ADDR		0x80
#define		DD_RAM_ADDR2	0xC0

void InitLCD(void);
void LCDSendCmd(unsigned char c);
void LCDSendStr(unsigned char* str);
void LCDSendChar(unsigned char c);


#ifdef	__cplusplus
}
#endif

#endif	/* LCDDRIVER_H */

