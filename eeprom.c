#include <xc.h>
#include <stdint.h>
#include "eeprom.h"

void ReadDataEEP(uint8_t* d_ptr, int baddr, int size)
{ int i;
  for (i=0; i < size; i++)
  {
    *d_ptr++ = eeprom_read(baddr++);
  }
}

void WriteDataEEP(uint8_t* d_ptr, int baddr, int size)
{ int i;
  for (i = 0; i < size; i++)
  {
    Write_b_eep( baddr++, *d_ptr++ );
  };
/*  {
    eeprom_write(baddr++, *d_ptr++);
  };*/
}

void Busy_eep ( void )
{

}

unsigned char Read_b_eep( unsigned int badd )
{
  EECON1 = 0;
  EEADR = badd & 0xFF;
  EEADRH = (badd & 0xFF00) >> 8;
  EECON1bits.RD = 1;
  return EEDATA;
}

void Write_b_eep( unsigned int badd, unsigned char bdat )
{
  EEADR = badd & 0xFF;
  EEADRH = (badd >> 8) & 0xFF;
  EEDATA = bdat;
  EECON1 = 0b00000100;
  di();
  EECON2 = 0x55;
  EECON2 = 0xAA;
  EECON1bits.WR = 1;
  ei();
//  while (PIR2bits.EEIF)
  while (EECON1bits.WR);
  EECON1bits.WREN = 0;
}
