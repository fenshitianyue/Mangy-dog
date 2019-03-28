#pragma once

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "common.h"

class Client{
public:
  ~Client(){
    Close();
  }
  void Start();
  void Close();
private:

};
