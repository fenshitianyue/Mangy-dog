#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
//#include <assert.h>

#define SERVER_PORT 8888
const char* SERVER_IP = "0.0.0.0";

int main(int argc, char* argv[]) {
  struct sockaddr_in serveraddr;  
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = PF_INET;
  serveraddr.sin_port = htons(SERVER_PORT);
  inet_pton(AF_INET, SERVER_IP , &serveraddr.sin_addr);
  
  int listener = socket(PF_INET, SOCK_STREAM, 0);
  if(listener < 0){
  
  }
  printf("listen socket created \n");
  if(bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){

  } 

  int ret = listen(listener, 5);
  if(ret < 0){

  }
  printf("Start to listen: %s\n", SERVER_IP);
  return 0;
}

