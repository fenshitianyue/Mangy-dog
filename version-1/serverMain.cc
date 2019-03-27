#include <iostream>
#include "common.h"
#include "server.h"

void Server::Init(){
  //1.Create a listener socket
  struct sockaddr_in serveraddr;  
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, SERVER_IP , &serveraddr.sin_addr);
 
  //2.Binding address
  int listener = socket(PF_INET, SOCK_STREAM, 0);
  CheckPrint(listener, "socket");
  std::cout << "listen socket created" << std::endl;
  int ret = bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
  CheckPrint(ret, "bind");
  
  //3.Listening to the connection
  ret = listen(listener, 5);
  CheckPrint(ret, "listen");

  //4.Create event table
  //5.Add listening "fd" to "Epoll fd"
}

void Server::Start(){
  //1.Init server
  Init();
  //2.Enter the main loop
  while(1){
    //3.Gets the ready event
    //4.The loop handlers all events
    //4.1 If it is a new connection, accept the connection 
    //and add it to "Epoll fd"
    //4.2 If it is new, the message is broadcast to another 
    //client
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
    printf("Client ID:%d close.Number of people online now: %d\n", clientfd, static_cast<int>(clients_list.size()));
  }else{
    //3.Determine if there are other clients in the chat room
    if(1 == clients_list.size()){
      send(clientfd, SERVER_WELCOME, sizeof(SERVER_WELCOME), 0); //maybe have bug
      return len;
    }
    //4.Format the content of the message sent
    sprintf(message, SERVER_MESSAGE, clientfd, buf);
    //5.Traverse the client list and send messages in turn
    for(const auto& it : clients_list){
      if(it != clientfd){
        if(send(it, message, BUF_SIZE, 0) < 0) {perror("send"); exit(EXIT_FAILURE);}
      }
    }
  }
  return len;
}
