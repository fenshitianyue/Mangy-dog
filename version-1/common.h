#pragma once 
#include <cstdlib>
#include <fcntl.h>
#include <sys/epoll.h>

/**********************           some config             *************************/
#define SERVER_PORT 8888
const char* SERVER_IP = "127.0.0.1";

//start message 
#define SERVER_WELCOME "Welcome you join to the chat room! your chat ID : Clitnt #%d"
//quit message
#define SERVER_QUIT "bye bye!"
//remind message
#define REMIND "There is only one int the chat room!"
//server normal reply
#define SERVER_MESSAGE "Client ID:%d say:%s"

//buffer size 
#define BUF_SIZE 0xFFFF
//message cache size
#define MESSAGE_SIZE 1024
//maximum number of handlers supported by "Epoll"
#define EPOLL_SIZE 4096
/*config end*/


/**********************           some common function            *************************/
//check status if failure to print failure message 
//parameter@num : external function return value
//parameter@info : external function name
void CheckPrint(const int num, const char* info);


//set the file descriptor to nonblocking
//parameter@fd : file descripter
#define SETNONBLOCK(fd) (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK))


//add a file descriptor to the table of kernel events for epoll
//parameter@epollfd : epoll descripter
//parameter@fd : socket descripter
//parameter@enable_et : enable_et = true -> epoll use ET option, otherwise use LT option
void addfd(int epollfd, int fd, bool enable_et);

