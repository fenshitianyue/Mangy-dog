#pragma once

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <list>

class Server{
public:
  Server();
  ~Server();
  //void Init();
  void Close();
  void Start();
  int SendBroadcastMessage(int clientfd);
private:
  std::list<int> clients_list;
  
};
