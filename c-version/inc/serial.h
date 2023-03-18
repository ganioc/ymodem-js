#ifndef SERIAL_H
#define SERIAL_H

/*
 * Return
 * status: OK, DATA, TIMEOUT,
 * length: 数据的长度, 128+5, 1024+5, 3个头，2个CRC,
 */
int receive_packet(int fd_port, char *buffer, int len, int timeout_secs);

#endif