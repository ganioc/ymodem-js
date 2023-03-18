
#include "crc16.h"

static uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
  uint32_t crc = crcIn;
  uint32_t inNum = (uint32_t)byte | 0x100;

  do
  {
    crc <<= 1;
    crc = crc & 0xFFFFFFFF;
    inNum <<= 1;
    inNum = inNum & 0xFFFFFFFF;

    if (inNum & 0x100)
    {
      ++crc;
    }

    if (crc & 0x10000)
    {
      crc ^= 0x1021;
      crc = crc & 0xFFFFFFFF;
    }

  } while (!(inNum & 0x10000));

  return (uint16_t)crc & 0xffff;
}

uint16_t crc16(char *data, int size)
{
  uint16_t crc = 0;

  for (int i = 0; i < size; i++)
  {
    crc = UpdateCRC16(crc, data[i]);
  }
  crc = UpdateCRC16(crc, 0);
  crc = UpdateCRC16(crc, 0);

  return crc & 0xffff;
}