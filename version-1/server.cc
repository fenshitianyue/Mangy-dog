#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "server.h"
#include "common.h"

void Server::Start(){
  /******************      Init server      *********************/
  //1.Create a listener socket
  struct sockaddr_in serveraddr;  
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, "127.0.0.1" , &serveraddr.sin_addr);
 
  //2.Binding address
  int listener = socket(PF_INET, SOCK_STREAM, 0);
  CheckPrint(listener, "socket");
  std::cout << "listen socket created" << std::endl;
  int ret = bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
  CheckPrint(ret, "bind");
  
  //3.Listening to the connection
  ret = listen(listener, 5);
  CheckPrint(ret, "listen");

  //4.Create event table in kernel
  int epfd = epoll_create(EPOLL_SIZE);
  CheckPrint(epfd, "epoll_create");
  std::cout << "epoll created, epollfd : " << epfd << std::endl;
  static struct epoll_event events[EPOLL_SIZE];
  //5.Add listening "fd" to "Epoll fd"
  addfd(epfd, listener, true);
  /*Init server OK*/

  //1.Enter the main loop
  while(1){
    //2.Gets the ready event
    int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
    CheckPrint(epoll_events_count, "epoll_wait");
    std::cout << "epoll_events_count : " << epoll_events_count << std::endl;
    //3.The loop handlers all events
    for(int i = 0; i < epoll_events_count; ++i){
      int sockfd = events[i].data.fd;
      //3.1 If it is a new connection, accept the connection and add it to "Epoll fd"
      if(sockfd == listener){
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        int clientfd = accept(listener, (struct sockaddr*)&client_address, &client_addr_len);
        std::cout << "client connection from: " 
                  << inet_ntoa(client_address.sin_addr)
                  << ":"
                  << ntohs(client_address.sin_port)
                  << " clientfd : "
                  << clientfd
                  << std::endl;
        //if(send(clientfd, CLIENT_MESSAGE_PREFEX, clientfd, 0) < 0) { perror("send"); exit(EXIT_FAILURE); } 
        //if(send(clientfd, "[Client ID:%d]:", clientfd, 0) < 0) { perror("send"); exit(EXIT_FAILURE); } 
        addfd(epfd, clientfd, true);
        clients_list.push_back(clientfd);
        std::cout << "Add new clientfd(" << clientfd << ") to epoll" << std::endl;
        std::cout << "Number of people online now: " << static_cast<int>(clients_list.size()) <<std::endl;
        //The server sends a welcome message
        // char message[BUF_SIZE] = {0};
        // sprintf(message, CLIENT_MESSAGE_PREFEX, clientfd);
        // int ret = send(clientfd, message, BUF_SIZE, 0);
        // CheckPrint(ret, "send");
      }else{
        //3.2 If it is new, the message is broadcast to another client
        int ret = SendBroadcastMessage(sockfd);
        CheckPrint(ret, "SendBroadcastMessage");
      }
    }
  }
}

int Server::SendBroadcastMessage(int clientfd){
  //1.Accept new message
  char buf[BUF_SIZE], message[BUF_SIZE];
  bzero(buf, BUF_SIZE);
  bzero(message, BUF_SIZE);
  printf("read from client(client ID = %d)\n", clientfd);
  int len = recv(clientfd, buf, BUF_SIZE, 0);
  //2.Determines whether the client terminates the connection
  if(0 == len){
    close(clientfd);
    clients_list.remove(clientfd);
    std::cout << "Client ID:" 
              << clientfd 
              << " close."
              << "Number of people online now: "
              << static_cast<int>(clients_list.size())
              << std::endl;
  }else{
    //3.Determine if there are other clients in the chat room
    if(1 == clients_list.size()){
      //send(clientfd, SERVER_WELCOME, sizeof(SERVER_WELCOME), 0); //maybe have bug
      return len;
    }
    //4.Format the content of the message sent
    sprintf(message, MESSAGE_PREFEX, clientfd, buf);
    //5.Traverse the client list and send messages in turn
    for(const auto& it : clients_list){
      if(it != clientfd){
        if(send(it, message, BUF_SIZE, 0) < 0) { perror("send"); exit(EXIT_FAILURE); }
      }
    }
  }
  return len;
}
