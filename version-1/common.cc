#include <iostream>
#include <cstdio>
#include "common.h"

void CheckPrint(const int num, const char* info){
  if(num < 0){
    perror(info);
    exit(EXIT_FAILURE);
  }
}

void addfd(int epollfd, int fd, bool enable_et){
  struct epoll_event ev;
  ev.data.fd = fd;
  ev.events = EPOLLIN;
  if(enable_et){
    ev.events = EPOLLIN | EPOLLET;
  }
  epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
  SETNONBLOCK(fd);
  std::cout << "fd added to epoll!" << std::endl;
}
