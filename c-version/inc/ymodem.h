#ifndef Y_MODEM_H
#define Y_MODEM_H

int sync_with_client(int fd_port, char *buffer, int timeout_times, int sync_times);
int send_file(int fd_port, char *buffer, int len);
#endif