/* 
 * File:   delay.h
 * Author: user
 *
 * Created on 2015. október 9., 13:30
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define		DELAY_1000us			498		//498*2+3 = 999us
#define		DELAY_125us				61		//61*2+3 = 125us
#define		DELAY_250us				123		//123*2+3 = 249us

void Delay(uint16_t cntr);
void DelayMs (uint16_t e);


#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

