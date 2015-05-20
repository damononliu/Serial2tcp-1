/*************************************************************************

  > File Name: serial.h
  > Author: lunan
  > Mail: 6616@shuncom.com 
  > Created Time: 2015年05月18日 星期一 18时09分09秒
 ************************************************************************/

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>

#define SERIAL_BUFFER_SIZE    200

extern int serial_init(char *path);

extern void sendCmd(int fd,uint8_t* buf, uint8_t len);

extern void serial_recv(int serial, short event, void* arg);

extern void register_sock_fd(int fd);

#endif
