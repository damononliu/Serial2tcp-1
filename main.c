/*************************************************************************
  > File Name: main.c
  > Author: lunan
  > Mail: 6616@shuncom.com 
  > Created Time: 2015年05月18日 星期一 18时08分34秒
 ************************************************************************/

#include<stdio.h>
#include <stdint.h>
#include <event.h>
#include "serial.h"
#include "socket.h"
#include "globalVariable.h"

int main(int argc,char *argv[])
{
	int serial_fd,sock_fd;
	char dev[] = "/dev/ttyATH0";
	struct event listen_socket;
	struct event listen_serial;

	serial_fd = serial_init(dev);
//	sendCmd(fd,test,5);
	sock_fd = socket_init();
	register_sock_fd(sock_fd);
	register_serial_fd(serial_fd);

	base = event_base_new();
	event_set(&listen_socket, sock_fd, EV_READ|EV_PERSIST, on_accept, NULL);
	event_set(&listen_serial, serial_fd, EV_READ|EV_PERSIST, serial_recv, NULL);
	event_base_set(base, &listen_socket);
	event_base_set(base, &listen_serial);
	event_add(&listen_socket, NULL);
	event_add(&listen_serial, NULL);
	event_base_dispatch(base);
	return 0;
}
