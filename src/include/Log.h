#ifndef _LOG_H_
#define _LOG_H_

#include <string>

class Log
{
public:
  Log();
  ~Log();

  static void Print(std::string std, ...);
  static void Print(const char* str, ...);
};

#endif