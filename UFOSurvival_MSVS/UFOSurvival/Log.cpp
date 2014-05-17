#include "Log.h"
#include <stdarg.h>

Log::Log()
{
}

Log::~Log()
{
}

void Print(const char* str, ...)
{
  va_list argptr;
  va_start(argptr, str);
  vfprintf(stderr, str, argptr);
  va_end(argptr);
}

void Log::Print(std::string str, ...)
{
  const char *c_str = str.c_str();

  va_list argptr;
  va_start(argptr, c_str);
  vfprintf(stderr, c_str, argptr);
  va_end(argptr);
}
