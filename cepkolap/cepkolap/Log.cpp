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
  printf(str.c_str());
}
