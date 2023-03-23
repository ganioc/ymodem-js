#include "packet.h"
#include "crc16.h"

#include <stdint.h>

#include "lib.h"

int get_zero_content(char *fileSymbol, int fileLen, char *buf)
{
  // char buf[128];
  char fileLenBuf[64];
  int i = 0;

  for (int i = 0; i < 128; i++)
  {
    buf[i] = 0;
  }

  sprintf(fileLenBuf, "%d", fileLen);

  i = 0;

  for (int j = 0; j < strlen(fileSymbol); j++)
  {
    buf[i++] = fileSymbol[j];
  }
  buf[i] = 0;

  for (int j = 0; j < strlen(fileLenBuf); j++)
  {
    buf[i++] = fileLenBuf[j];
  }
  return 128;
}
/* buf len = NORMAL_LEN + 3 + 2
 */
void get_normal_packet(int id, char *contentBuf, int len, char *buf)
{
  int i = 0;
  int contentLen;
  uint16_t crc;

  buf[i++] = SOH;
  buf[i++] = id;
  buf[i++] = 0xFF - id;

  contentLen = len;
  if (contentLen > NORMAL_LEN)
  {
    perror("Over normal packet size limit");
    exit(-1);
  }

  for (int j = 0; j < contentLen; j++)
  {
    buf[i++] = contentBuf[j];
  }
  while (i < NORMAL_LEN + 3)
  {
    buf[i++] = 0x1A;
  }

  // crc not finished
  crc = crc16(&buf[3], NORMAL_LEN);

  buf[i++] = (crc >> 8) & 0xFF;
  buf[i++] = crc & 0xFF;

  PRINTF("packet forming End i= %d\n", i);
}

void get_long_packet(int id, char *contentBuf, int len, char *buf)
{
  int i = 0;
  int contentLen;
  uint16_t crc;

  buf[i++] = STX;

  buf[i++] = id;
  buf[i++] = 0xFF - id;

  contentLen = len;

  if (contentLen > LONG_LEN)
  {
    perror("Over long packet size limit");
    exit(-1);
  }

  for (int j = 0; j < contentLen; j++)
  {
    buf[i++] = contentBuf[j];
  }

  while (i < NORMAL_LEN + 3)
  {
    buf[i++] = 0x1A;
  }

  crc = crc16(&buf[3], LONG_LEN);

  buf[i++] = (crc >> 8) & 0xFF;
  buf[i++] = crc & 0xFF;

  PRINTF("packet forming end i = %d\n", i);
}