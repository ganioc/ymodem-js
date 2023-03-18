#include "serial.h"
#include "lib.h"

#include <stdio.h>

int receive_packet(int fd_port, char *buffer, int len, int timeout_secs)
{
  int rxIndex = 0;
  int len_expected = len;
  int num_bytes = 0;
  int bytes_received = 0;

  while (timeout_secs-- > 0)
  {
    num_bytes = read(fd_port, buffer + bytes_received, len - bytes_received);
    bytes_received += num_bytes;

    if (bytes_received >= len_expected)
    {
      printf("Received Packet len: %d\n", bytes_received);
      print_rx_buf(buffer, bytes_received);
      return 0;
    }
    if (buffer[0] == packet.SOH)
    {
    }
  }
  return 1;
}