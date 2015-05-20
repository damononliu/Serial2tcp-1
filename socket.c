/*************************************************************************
  > File Name: socket.c
  > Author: lunan
  > Mail: 6616@shuncom.com 
  > Created Time: 2015年05月18日 星期一 18时09分25秒
 ************************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>

#include <event.h>
#include "globalVariable.h"
#include "serial.h"


#define PORT        7777
#define BACKLOG     5
#define MEM_SIZE    1024

struct event_base *base;
struct sock_ev {
	struct event* read_ev;
	struct event* write_ev;
	uint8_t* buffer;
};

int client_fd = 0;
int serial_fd = 0;

int check_client(int fd)
{
	struct tcp_info info; 
	int len=sizeof(info); 
	getsockopt(fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 
	if((info.tcpi_state==TCP_ESTABLISHED)){
		return 0;
	}else{
		return -1;
	}	
}
void register_serial_fd(int fd)
{
	serial_fd = fd;	
}

void release_sock_event(struct sock_ev* ev)
{
	event_del(ev->read_ev);
	free(ev->read_ev);
	free(ev->write_ev);
	free(ev->buffer);
	free(ev);
}

void on_write(int sock, short event, void* arg)
{
	char* buffer = (char*)arg;
	send(sock, buffer, strlen(buffer), 0);

	free(buffer);
}

void on_read(int sock, short event, void* arg)
{
	struct event* write_ev;
	int i,size;
	struct sock_ev* ev = (struct sock_ev*)arg;
	ev->buffer = (uint8_t*)malloc(MEM_SIZE);
	bzero(ev->buffer, MEM_SIZE);
	
	size = recv(sock, ev->buffer, MEM_SIZE, 0);
	for(i=0;i<size;i++){
		printf(" %02x ", ev->buffer[i]);
	}
	printf("\n");
	sendCmd(serial_fd,ev->buffer,size);

	if(size == 0){
//		j++;
//	if (j >= 10) {
		printf("break the client\n");
		release_sock_event(ev);
		close(sock);
		return;
//	}
	}
#if 0
	event_set(ev->write_ev, sock, EV_WRITE, on_write, ev->buffer);
	event_base_set(base, ev->write_ev);
	event_add(ev->write_ev, NULL);
#endif
}

void on_accept(int sock, short event, void* arg)
{
	struct sockaddr_in cli_addr;
	int sin_size;
	struct sock_ev* ev = (struct sock_ev*)malloc(sizeof(struct sock_ev));

	printf("on_accept\n");
	ev->read_ev = (struct event*)malloc(sizeof(struct event));
	ev->write_ev = (struct event*)malloc(sizeof(struct event));
	sin_size = sizeof(struct sockaddr_in);
	client_fd = accept(sock, (struct sockaddr *)&cli_addr, (socklen_t *)&sin_size);
	event_set(ev->read_ev, client_fd, EV_READ|EV_PERSIST, on_read, ev);
	event_base_set(base, ev->read_ev);
	event_add(ev->read_ev, NULL);
}

int socket_init()
{
	struct sockaddr_in my_addr;
	int sock;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bind(sock, (struct sockaddr*)&my_addr, sizeof(struct sockaddr));
	listen(sock, BACKLOG);

	return sock;
}
