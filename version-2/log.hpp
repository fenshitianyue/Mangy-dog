#pragma once

#include <iostream>
#include <string>

#define NORMAL 0
#define WARNING 1
#define ERROR 2

const char* log_level[] = {"Normal", "Warning", "Errir", NULL, };

void Log(std::string& msg, int level, std::string file_name, int line){
  std::cout << '[' << msg << ']' << '[' << log_level[level] << ']' << " : "
            << file_name << " : " << line << std::endl;
}

#define LOG(msg, level) Log(msg, level, __FILE__, __LINE__)

