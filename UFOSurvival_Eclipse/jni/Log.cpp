#include "Log.h"
#include <android/log.h>

Log::Log()
{
}

Log::~Log()
{
}

void Log::Print(std::string str, ...)
{
  const char* pMessage = str.c_str();

  va_list lVarArgs;
  va_start(lVarArgs, str);
  __android_log_vprint(ANDROID_LOG_WARN, "UFOSurvivalLog", pMessage, lVarArgs);
  __android_log_print(ANDROID_LOG_WARN, "UFOSurvivalLog", "\n");
  va_end(lVarArgs);
}

void Log::Print(const char* pMessage, ...)
{
  va_list lVarArgs;
  va_start(lVarArgs, pMessage);
  __android_log_vprint(ANDROID_LOG_WARN, "UFOSurvivalLog", pMessage, lVarArgs);
  __android_log_print(ANDROID_LOG_WARN, "UFOSurvivalLog", "\n");
  va_end(lVarArgs);
}
