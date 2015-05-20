/*************************************************************************
  > File Name: serial.c
  > Author: lunan
  > Mail: 6616@shuncom.com 
  > Created Time: 2015年05月18日 星期一 18时09分03秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <termios.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <event.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "serial.h"
#include "socket.h"

extern uint8_t scan_flag;

static int sock_fd;

struct serial_ev {
	struct event* read_ev;
	struct event* write_ev;
	char* buffer;
};

void release_serial_event(struct serial_ev* ev)
{
	event_del(ev->read_ev);
	free(ev->read_ev);
	free(ev->write_ev);
	free(ev->buffer);
	free(ev);
}

void calcFcs(uint8_t *msg, int size)
{
	uint8_t result = 0;
	int idx = 1; 		//skip SOF
	int len = (size - 1); 	// skip FCS

	while ((len--) != 0) {
		result ^= msg[idx++];
	}

	msg[(size - 1)] = result;
}

static inline int readn(int fd, uint8_t *buf, uint8_t len)
{
	int r, nread = 0;

	assert(fd > 0);
	while (nread < len) {
		if ((r = read(fd, buf + nread, len - nread)) < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			break;	
		}


		if (!r)
			break;
		nread += r;
	}

	return nread;
}

static inline int writen(int fd, uint8_t *buf, uint8_t len)
{
	int r, nwrite = 0;

	assert(fd > 0);
	while (nwrite < len) {
		if ((r = write(fd, buf + nwrite, len - nwrite)) < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
		}

		if (!r)
			break;

		nwrite += r;
	}

	return nwrite;
}

void sendCmd(int fd,uint8_t* buf, uint8_t len)
{
//	scan_flag |= ((uint8_t)1 << SERIAL_FLAG_BIT);
	assert(fd > 0);
//	calcFcs(buf, len);

	printf(">>>> ");
	int i;
	for (i = 0; i < len; i++)
		printf("%02X ", buf[i]);

	printf("\n");

	writen(fd, buf, len);

//	scan_flag &= ~((uint8_t)1 << SERIAL_FLAG_BIT);
}

void register_sock_fd(int fd)
{
	sock_fd = fd;	
}

int serial_init(char *path)
{
	int fd;
	struct termios tio;

	fd = open(path, O_RDWR | O_NOCTTY);
	assert(fd > 0);

	ioctl(fd, TIOCEXCL);

	cfmakeraw(&tio);
	tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tio.c_cflag = B38400 | CREAD;
	tio.c_cc[VTIME] = 1;
	tio.c_cc[VMIN] = 0;

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &tio);

	return fd;
}

void serial_recv(int fd, short event, void* arg)
{
	uint8_t i,*buffer = NULL;
	int size;
	buffer = (uint8_t *)malloc(SERIAL_BUFFER_SIZE);
	size = readn(fd,buffer,SERIAL_BUFFER_SIZE);
	printf("serial in:");
	for(i=0;i<size;i++){
		printf(" %02x ",buffer[i]);
	}
	printf("\n");
	if(check_client(client_fd)==0){
		send(client_fd,buffer,size,0);
		printf("send it to client;\n");
	}
	else
		printf("client have break;\n");

	free(buffer);
}

