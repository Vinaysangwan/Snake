#pragma once

// #############################################################################
//                           Logger
// #############################################################################
#ifdef _WIN32
  #define DEBUG_BREAK() __debugbreak()
#elif __APPLE__
  #define DEBUG_BREAK() __builtin_debugtrap()
#elif __linux__
  #define DEBUG_BREAK() __builtin_trap()
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
  char textBuffer[8192];

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

// #############################################################################
//                           File
// #############################################################################
char* read_file(const char* filePath);

// #############################################################################
//                           Vectors
// #############################################################################
struct Vec2
{
  float x;
  float y;
};

struct IVec2
{
  int x;
  int y;
};

struct Vec3
{
  float x;
  float y;
  float z;
};

struct Vec4
{
  union
  {
    struct
    {
      float x;
      float y;
      float z;
      float w;
    };

    struct
    {
      float r;
      float g;
      float b;
      float a;
    };
  };
};

Vec2 vec2i_f(IVec2 vec);

// #############################################################################
//                           Mat4
// #############################################################################
struct Mat4
{
  union
  {
    float values[16];

    struct
    {
      float ax;
      float bx;
      float cx;
      float dx;

      float ay;
      float by;
      float cy;
      float dy;

      float az;
      float bz;
      float cz;
      float dz;

      float aw;
      float bw;
      float cw;
      float dw;
    };
  };

  inline float &operator[](int idx)
  {
    return values[idx];
  }
};

Mat4 projection_orthographic(float left, float right, float top, float bottom);
