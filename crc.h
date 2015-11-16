/* 
 * File:   crc.h
 * Author: user
 *
 * Created on 2015. július 27., 11:14
 */

#ifndef CRC_H
#define	CRC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CRC16 0x8005

typedef uint8_t (*READ_BYTE_FUNCTION)(uint16_t address);
uint16_t gen_crc16(READ_BYTE_FUNCTION READ_FUNCTION, uint16_t count, uint16_t initCRC);

#ifdef	__cplusplus
}
#endif

#endif	/* CRC_H */

