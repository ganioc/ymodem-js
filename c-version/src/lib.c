#include "lib.h"

void print_rx_buf(char *buffer, int len)
{
  printf("print_rx_buf: %d\n", len);
  for (int i = 0; i < len; i += 16)
  {
    int upper = (len < i + 16) ? len : i + 16;

    printf("0x%02X: ", i);
    for (int j = i; j < upper; j++)
    {
      printf("%02x ", buffer[j]);
    }
    printf("\n");
  }
}