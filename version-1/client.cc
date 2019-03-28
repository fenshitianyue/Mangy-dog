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
  //3.Create pipe. pipe_fd[0] -> father process read | pipe_fd[1] -> child process write
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
  
  //Indicates whether the client is working properly
  bool IsClientWork = true;

  //chat message buffer
  char message[BUF_SIZE] = {0};
  int pid = fork();
  CheckPrint(pid, "fork");
  if(0 == pid){ //child process
    close(pipe_fd[0]);
    std::cout << "Input 'exit' to exit the chat room" << std::endl;
    while(IsClientWork){
      fgets(message, BUF_SIZE, stdin);
      //client input 'exit', exit chat room
      if(0 == strncasecmp(message, "exit", strlen("exit"))){
        IsClientWork = false;
      }else{ //Otherwise the child process writes the information to the pipe
        CheckPrint(write(pipe_fd[1], message, strlen(message) - 1), "write");
      }
    }
  }else { //father process
    close(pipe_fd[1]);
    while(IsClientWork){
      int epoll_events_count = epoll_wait(epfd, events, 2, -1);
      for(int i = 0; i < epoll_events_count; ++i){
        if(events[i].data.fd == sock){ //The server sent the message
          int ret = recv(sock, message, BUF_SIZE, 0); 
          CheckPrint(ret, "recv");
          if(0 == ret){
            std::cout << "Server close connection: " << sock << std::endl;
            close(sock);
            IsClientWork = false;
          }else{
            std::cout << message << std::endl;
          }
        }else{//The child writes to event and the parent processes and sends it to the server
          int ret = read(events[i].data.fd, message, BUF_SIZE);
          CheckPrint(ret, "read");
          if(0 == ret){
            IsClientWork = false;
          }else{
            send(sock, message, BUF_SIZE, 0);
          }
        }
      }//end for
    }//end while
  }
  if(pid){
    close(pipe_fd[0]);
    close(sock);
  }else{
    close(pipe_fd[1]); 
  }
}

  
