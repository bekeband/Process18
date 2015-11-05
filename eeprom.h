/* 
 * File:   eeprom.h
 * Author: user
 *
 * Created on 2015. október 1., 11:53
 */

#ifndef EEPROM_H
#define	EEPROM_H

#ifdef	__cplusplus
extern "C" {
#endif

void ReadDataEEP(char* d_ptr, int baddr, int size);
void WriteDataEEP(unsigned char* d_ptr, int baddr, int size);


#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

