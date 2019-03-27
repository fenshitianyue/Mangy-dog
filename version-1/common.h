#pragma once 
#include <stdio.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define SERVER_PORT 8888
const char* SERVER_IP = "0.0.0.0";

//start message 
//TODO
//quit message
//TODO


//message cache size
#define MESSAGE_SIZE 1024


//set the file descriptor to nonblocking
#define SETNONBLOCK(fd) (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK))
//void setnonblock(int fd){
//  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
//}


//add a file descriptor to the table of kernel events for epoll
void addfd(int epollfd, int fd, bool enable_et){
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  if(enable_et){
    ev.events = EPOLLIN | EPOLLET;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
  SETNONBLOCK(fd);
  printf("fd added to epoll!\n\n");
}
