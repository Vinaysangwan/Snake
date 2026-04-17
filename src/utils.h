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
inline char* format_text(const char* msg, Args... args)
{
  static char textBuffer[8192];

  if constexpr (sizeof...(args) == 0)
  {
    snprintf(textBuffer, sizeof(textBuffer), "%s", msg);
  }
  else
  {
    snprintf(textBuffer, sizeof(textBuffer), msg, args...);
  }

  return &textBuffer[0];
}

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
//                           File Handeling
// #############################################################################
char* read_file(const char* filePath);

// #############################################################################
//                           Vectors
// #############################################################################
struct Vec2
{
  float x;
  float y;

  constexpr Vec2() : x(0), y(0) { }
  constexpr Vec2(float x, float y) : x(x), y(y) { }
};

struct IVec2
{
  int x;
  int y;

  constexpr IVec2() : x(0), y(0) { }
  constexpr IVec2(int x, int y) : x(x), y(y) { }
};

struct Vec3
{
  union
  {
    struct
    {
      float x;
      float y;
      float z;
    };

    struct
    {
      float r;
      float g;
      float b;
    };
  };
};

struct IVec3
{
  union
  {
    struct
    {
      int x;
      int y;
      int z;
    };

    struct
    {
      int r;
      int g;
      int b;
    };
  };
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

struct IVec4
{
  union
  {
    struct
    {
      int x;
      int y;
      int z;
      int w;
    };

    struct
    {
      int r;
      int g;
      int b;
      int a;
    };
  };
};

Vec2 ivec2_f(const IVec2 &vec);

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

Mat4 projection_orthographic_matrix(float left, float right, float top, float bottom);
Mat4 view_matrix(Vec2 pos, Vec2 size, float rot, int screenW, int screenH);

// #############################################################################
//                           Rects
// #############################################################################
struct Rect
{
  Vec2 pos;
  Vec2 size;
};

struct IRect
{
  IVec2 pos;
  IVec2 size;
};

// #############################################################################
//                           Math
// #############################################################################
constexpr float DEG_2_RAD = 0.0174532925199433;

template <typename T>
inline constexpr T min(T a, T b)
{
  return a > b ? b : a;
}

template <typename T>
inline constexpr T max(T a, T b)
{
  return a > b ? a : b;
}

template <typename T>
inline constexpr T clamp(T v, T l, T h)
{
  return max(min(v, h), l);
}

template <typename T>
inline constexpr T abs(T a)
{
  return a < 0 ? -a : a;
}

inline constexpr int BIT(int x)
{
  return 1 << x;
}

// #############################################################################
//                           Random Functions
// #############################################################################
void random_init_seed();
int random_get_int(int min, int max);
float random_get_float(float max = 1.0f);

// #############################################################################
//                           Collisions
// #############################################################################
bool collision_rects(const Rect &a, const Rect &b);
bool collision_irects(const IRect &a, const IRect &b);
