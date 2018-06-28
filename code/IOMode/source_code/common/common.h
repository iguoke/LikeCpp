#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string.h>

void 
request_add(int count);

int 
init_print_thread();

void 
wait_print_thread();

void
set_nonblock(int fd);

int
make_socket(int asyn);

void
set_block_filedes_timeout(int filedes);

int
client_write(int filedes);

int 
server_write(int filedes);

int
client_read(int filedes);

int
server_read(int filedes);

void* 
print_count(void* arg);

int
make_client_socket();

void 
connect_server(int client_sock);

#endif
