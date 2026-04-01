#pragma once

// #############################################################################
//                           Logger
// #############################################################################
#ifdef _WIN32
  #define DEBUG_BREAK() __debugbreak()
#endif

enum LoggerColor
{
  LOGGER_COLOR_GREEN = 32,
  LOGGER_COLOR_YELLOW = 33,
  LOGGER_COLOR_RED = 31,
};

template<typename ...Args>
inline void logger(LoggerColor color, const char* prefix, const char* msg, Args... args)
{
  char textBuffer[8192] = {};

  if constexpr (sizeof...(args) == 0)
  {
    snprintf(textBuffer, sizeof(textBuffer), "%s", msg);
  }
  else
  {
    snprintf(textBuffer, sizeof(textBuffer), msg, args...);
  }

  printf("\x1b[%dm%s %s\x1b[0m\n", (int)color, prefix, textBuffer);
}

#define SN_INFO(msg, ...) logger(LOGGER_COLOR_GREEN, "INFO: ", msg, ##__VA_ARGS__)
#define SN_WARN(msg, ...) logger(LOGGER_COLOR_YELLOW, "WARN: ", msg, ##__VA_ARGS__)
#define SN_ERROR(msg, ...) logger(LOGGER_COLOR_RED, "ERROR: ", msg, ##__VA_ARGS__)
#define SN_ASSERT(x, msg, ...)      \
{                                   \
  if(!(x))                          \
  {                                 \
    SN_ERROR(msg, ##__VA_ARGS__);   \
    DEBUG_BREAK();                  \
  }                                 \
}
