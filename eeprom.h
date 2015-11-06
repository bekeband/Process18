/* 
 * File:   eeprom.h
 * Author: user
 *
 * Created on 2015. okt�ber 1., 11:53
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

void ReadDataEEP(uint8_t* d_ptr, int baddr, int size);
void WriteDataEEP(uint8_t* d_ptr, int baddr, int size);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

