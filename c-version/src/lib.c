#include "lib.h"

void print_rx_buf(char *buffer, int len)
{
  printf("print_rx_buf: %d\n", len);
  for (int i = 0; i < len; i += 16)
  {
    int upper = (len < i + 16) ? len : i + 16;
    for (int j = i; j < upper; j++)
    {
      printf("0x%02X ", buffer[j]);
    }
    printf("\n");
  }
}