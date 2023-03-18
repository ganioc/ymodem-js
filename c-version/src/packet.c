#include "packet.h"

void get_zero_content(char *fileSymbol, int fileLen, char *buf)
{
  // char buf[128];
  char fileLenBuf[32];
  int i = 0;

  for (int i = 0; i < 128; i++)
  {
    buf[i] = 0;
  }

  sprintf(fileLenBuf, "%d", fileLen);

  for (int j = 0; j < strlen(fileSymbol); j++)
  {
    buf[i++] = fileSymbol[j];
  }
  buf[i] = 0;

  for (int j = 0; j < strlen(fileLenBuf); j++)
  {
    buf[i++] = fileLenBuf[j];
  }
}
/* buf len = NORMAL_LEN + 3 + 2
 */
void get_normal_packet(int id, char *contentBuf, char *buf)
{
  int i = 0;
  int contentLen;

  buf[i++] = SOH;
  buf[i++] = id;
  buf[i++] = 0xFF - id;

  contentLen = strlen(contentBuf);
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
}