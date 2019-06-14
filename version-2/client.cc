#include <iostream>
#include "client.h"

client::client(std::string server_ip, short server_port){
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(server_ip.data()); 
  server.sin_port = htons(server_port);
}



client::~client(){

}


