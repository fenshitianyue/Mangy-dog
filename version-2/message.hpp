#pragma once 

#include <iostream>
#include <string>
//TODO

#define NORMAL_TYPE 0
#define LOGIN_TYPE 1

class Message{
public:
  Message(){}
  Message(const std::string&n, const std::string& s, const std::string& t,\
      const unsigned int id, unsigned int type) :nick_name(n),school(s),text(t),id(id),type(type) {}
  const std::string& NickName(){
    return nick_name;
  }
  const std::string& School(){
    return school;
  }
  const std::string& Text(){
    return text;
  }
  const unsigned int& Id(){
    return id;
  }
  const unsigned int& Type(){
    return type;
  }
private:
  std::string nick_name;
  std::string school;
  std::string text;
  unsigned int id;
  unsigned int type;
};

