#include "client.h"

void Client::Start(){
  /********************        Connect server        ***********************/
  //1.Create socket
  struct sockaddr_in serveraddr;
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);
  int sock= socket(PF_INET, SOCK_STREAM, 0);
  CheckPrint(sock, "socket");
  //2.Connect server 
  if(connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
    perror("connect");
    exit(EXIT_FAILURE);
  }
  //3.Create pipe. pipe_fd[0] -> father process read | pipe_fd[1] -> son process write
  int pipe_fd[2];
  if(pipe(pipe_fd) < 0) { perror("pipe"); exit(EXIT_FAILURE); }
  //4.Create Epoll
  int epfd = epoll_create(EPOLL_SIZE);
  CheckPrint(epfd, "epoll_create");
  static struct epoll_event events[2];
  //5.Add the sock and pipe read-end descriptors to the kernel event table
  addfd(epfd, sock, true);
  addfd(epfd, pipe_fd[0], true);
  /* Server connection successful */
  
}

  
