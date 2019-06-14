#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class client{
public:
  client(std::string server_ip, short server_port);
  void client_init();
  void recv_data(std::string& out_str);
  void send_data(std::string& in_str);
  ~client();
private:
  int sock;
  sockaddr_in server;
};

