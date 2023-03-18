#ifndef PACKET_H
#define PACKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SOH 0x01
/* start of 128-byte data packet */
#define STX 0x02
/* start of 1024-byte data packet */
#define EOT 0x04
/* end of transmission */
#define ACK 0x06
/* acknowledge */
#define NAK 0x15
/* negative acknowledge */
#define CA 0x18
/* two of these in succession aborts transfer */
#define CRC16 0x43
/* 'C' == 0x43, request 16-bit CRC */
#define NEGATIVE_BYTE 0xFF

#define ABORT1 0x41
/* 'A' == 0x41, abort by user */
#define ABORT2 0x61
/* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT 10000
#define DOWNLOAD_TIMEOUT 1000
/* One second retry delay */
#define MAX_ERRORS 10

#define NORMAL_LEN 128
#define LONG_LEN 1024
#define DATA_INDEX 3

#define bUse1K 1

void get_zero_content(char *fileSymbol, int fileLen, char *buf);
void get_normal_packet(int id, char *contentBuf, int len, char *buf);
void get_long_packet(int id, char *contentBuf, int len, char *buf);
#endif