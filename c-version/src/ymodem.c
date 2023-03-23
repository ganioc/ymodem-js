#include "ymodem.h"
#include "packet.h"
#include "serial.h"
#include "lib.h"

#include <stdio.h>
#include <unistd.h>

int sync_with_client(int fd_port, int timeout_times, int sync_times)
{
  char buffer[3];

  int counter_sync = 0;
  int counter_timeout = 0;

  while (counter_timeout < timeout_times)
  {
    printf("Sync with Rx counter: %d\n", counter_sync);
    int len = receive_packet(fd_port, buffer, 1, 1);
    printf("sync len: %d\n", len);
    if (len == 1)
    {
      print_rx_buf(buffer, 1);
      if (buffer[0] == CRC16)
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
static int sendBlock0(int port, int id, char *fileName, int fileLen)
{
  char filename_len_buf[128]={0};
  char buf[128 + 2 + 3] = {0};
  int file_name_len_buf_size = 0;

  file_name_len_buf_size = get_zero_content(fileName, fileLen, filename_len_buf);

  get_normal_packet(id, filename_len_buf, file_name_len_buf_size, buf);

  printf("buf size: %ld\n", sizeof(buf));

  printf("- Send out blockZero\n");

  write(port, buf, sizeof(buf));

  print_rx_buf(buf, sizeof(buf));

  int len = receive_packet(port, buf, 2, 2);
  if (len == 2 && buf[0] == ACK && buf[1] == CRC16)
  {
    printf("Received ACK OK\n");
    return 0;
  }
  else
  {
    printf("Received wrong ACK\n");
    return -1;
  }
}
/*
 * send bin file content
 */
static int sendBlockFile(int fd_port, char *buf, int len)
{
  int nInterval = (bUse1K == 1) ? 1024 : 128;
  int errors = 0;
  char payloadBuf[1024 + 5] = {0};
  char block[1024 + 5] = {0};
  char rxBuf[16];

  for (int i = 0; i < len; i += nInterval)
  {
    if (errors > 5)
    {
      printf("Sending blocks failed\n");
      return -1;
    }
    for(int m = 0; m< 1024+5; m++){
      payloadBuf[m] = 0;
      block[m] = 0;
    }

    printf("\n- Send block: %d\n", i / nInterval + 1);
    int upper = (len < i + nInterval) ? len : i + nInterval;
    for (int j = i; j < upper; j++)
    {
      payloadBuf[j - i] = buf[j];
    }

    int id = i / nInterval + 1;
    if (bUse1K == 1)
    {
      get_long_packet(id, payloadBuf, LONG_LEN, block);
      write(fd_port, block, LONG_LEN + 5);
      print_rx_buf(block, LONG_LEN + 5);
    }
    else
    {
      get_normal_packet(id, payloadBuf, NORMAL_LEN, block);
      write(fd_port, block, NORMAL_LEN + 5);
      print_rx_buf(block, NORMAL_LEN + 5);
    }

    int len = read(fd_port, rxBuf, 1);
    if (len == 1)
    {
      printf("Got resp\n");
      print_rx_buf(rxBuf, 1);
    }
    else
    {
      printf("Got no resp\n");
      errors++;
      i -= nInterval;
      continue;
    }

    if (rxBuf[0] == CA)
    {
      printf("Write to Flash failed");
      return -1;
    }
    else if (rxBuf[0] != ACK)
    {
      printf("no Ack\n");
      errors++;
      i -= nInterval;

      if (errors > 5)
      {
        printf("errors num > 5\n");
        return -1;
      }
      continue;
    }
    printf("- Send block: %d succeed!\n", id);
  }
  return 0;
}
int sendBlockEOT(int fd_port)
{
  char buf[1];
  printf("-- Send EOT\n");

  buf[0] = EOT;
  write(fd_port, buf, 1);

  int len = receive_packet(fd_port, buf, 1, 1);
  if (len != 1 || buf[0] != NAK)
  {
    printf("Wrong ack\n");
    return -1;
  }
  buf[0] = EOT;
  write(fd_port, buf, 1);
  len = receive_packet(fd_port, buf, 1, 1);
  if (len != 1 || buf[0] != ACK)
  {
    printf("Wrong 2nd ack\n");
    return -1;
  }

  len = receive_packet(fd_port, buf, 1, 1);
  if (len != 1 || buf[0] != CRC16)
  {
    printf("Wrong 3rd CRC16\n");
    return -1;
  }
  return 0;
}
static int sendBlockLast(int fd_port)
{
  char content[NORMAL_LEN] = {0};
  char blockLast[NORMAL_LEN + 5] = {0};
  char rxBuf[2] = {0};
  int errors = 0;

  get_normal_packet(0, content, NORMAL_LEN, blockLast);

  printf("Send last block to finish session\n");

  do
  {
    if (errors > 3)
    {
      printf("Can not finish session\n");
      return -1;
    }
    write(fd_port, blockLast, NORMAL_LEN + 5);
    print_rx_buf(blockLast, NORMAL_LEN + 5);

    int len = receive_packet(fd_port, rxBuf, 1, 1);
    if (len == 1 && rxBuf[0] == ACK)
    {
      print_rx_buf(rxBuf, 1);
      break;
    }
    else
    {
      printf("no response\n");
      errors++;
      continue;
    }
  } while (1);
  return 0;
}
int send_file(int fd_port, char *buffer, int len, char *fileName)
{
  int result;
  int id = 0;

  printf("send file() ...\n");

  result = sendBlock0(fd_port, id, fileName, len);
  if (result != 0)
  {
    printf("-- Send block 0 failed\n");
    return -1;
  }else{
    printf("--- send block 0 finished\n");
  }
  // id++
  result = sendBlockFile(fd_port, buffer, len);
  if (result != 0)
  {
    printf("-- Send file failed\n");
    return -1;
  }

  result = sendBlockEOT(fd_port);
  if (result != 0)
  {
    printf("-- Send EOT failed\n");
    return -1;
  }

  result = sendBlockLast(fd_port);
  if (result != 0)
  {
    printf("-- Send last block failed\n");
    return -1;
  }

  return 0;
}