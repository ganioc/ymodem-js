#include "serial.h"
#include "lib.h"
#include "packet.h"

#include <stdio.h>
#include <unistd.h>

/**
 * @brief
 *
 * @param fd_port
 * @param buffer
 * @param len
 * @param timeout_secs
 * @return received length
 */
int receive_packet(int fd_port, char *buffer, int len, int timeout_secs)
{
  // int rxIndex = 0;
  int len_expected = len;
  int num_bytes = 0;
  int bytes_received = 0;
  int header_touched = 0;

  while (timeout_secs-- > 0)
  {
    num_bytes = read(fd_port, buffer + bytes_received, len - bytes_received);

    PRINTF("receive_packet, num: %d\n", num_bytes);
    bytes_received += num_bytes;

    if (header_touched == 0 && num_bytes > 0)
    {
      if (buffer[0] == SOH)
      {
        len_expected = 128 + 5;
      }
      else if (buffer[0] == STX)
      {
        len_expected = 1024 + 5;
      }
      header_touched = 1;
    }

    if (bytes_received >= len_expected)
    {
      PRINTF("Received Packet len: %d\n", bytes_received);
      PRINT_RX_BUF(buffer, bytes_received);
      return len_expected;
    }
  }
  return -1;
}