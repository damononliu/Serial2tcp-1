/*************************************************************************

    > File Name: socket.h
    > Author: lunan
    > Mail: 6616@shuncom.com 
    > Created Time: 2015年05月18日 星期一 18时09分19秒
 ************************************************************************/

#ifndef _SOCKET_H_
#define _SOCKET_H_

#define MAX_CLIENT_NUM   20

extern int client_fd;

extern int socket_init();

extern void on_accept(int sock, short event, void* arg);

extern void register_serial_fd(int fd);

extern int check_client(int fd);

#endif
