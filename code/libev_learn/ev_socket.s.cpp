/*************************************************************************
	> File Name: ev_socket.s.cpp
	> Author: genglut
	> Mail: genglut@163.com
	> Created Time: 2018年06月20日 星期三 14时19分59秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<netinet/in.h>
#define PORT_NO 3033
#define BUFFER_SIZE 1024

using namespace std;
int total_clients=0;

void accept_cb(struct ev_loop *loop,struct ev_io *watcher,int revents);
void read_cb(struct ev_loop *loop,struct ev_io *watcher,int revents);

int main()
{
	struct ev_loop *loop=ev_default_loop;
}


