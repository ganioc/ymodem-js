#include "ymodem.h"

int sync_with_client(int fd_port, char *buffer, int timeout_times, int sync_times)
{
  int counter_sync = 0;
  int counter_timeout = 0;

  while (counter_timeout < timeout_times)
  {
    printf("Sync with Rx counter: %d\n", counter_sync);
    int result = receive_packet(fd_port, buffer, 1, 1);
    printf("result: %d\n", result);
    if (result == 0)
    {
      print_rx_buf();
      if (buffer[0] == Packet.CRC16)
      {
        counter_sync++;
      }
      if (counter_sync >= sync_times)
      {
        // sync succeeds
        return 0;
      }
    }
    else
    {
      counter_timeout++;
    }
  }
  // sync failed
  return 1;
}

int send_file(int fd_port, char *buffer, int len)
{
}