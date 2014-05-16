#include "Log.h"
#include <iostream>

Log::Log()
{
}

Log::~Log()
{
}

void Log::Print(std::string str)
{
  std::cout << str.c_str();
}
