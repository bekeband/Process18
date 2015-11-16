
#include <stdint.h>
#include "crc.h"

uint16_t gen_crc16(READ_BYTE_FUNCTION READ_FUNCTION, uint16_t count, uint16_t initCRC)
{
  uint16_t crc;
  uint8_t i;

  uint16_t address = 0;
  uint8_t next_byte;
  uint8_t* data = &next_byte;

  crc = initCRC;
  while((count) > 0 )
    {
    next_byte = READ_FUNCTION(address++);
    crc = crc ^ ((uint16_t) *data/*++*/ << 8);
      for(i = 0; i < 8; i++)
        {
          if( crc & 0x8000 )
            {
              crc = (crc << 1) ^ 0x1021;
            }
          else
            {
              crc = crc << 1;
            }
        }
    count--;
    }
  return crc;
}

