#include "pch.h"
#include "utils.h"

// #############################################################################
//                           File
// #############################################################################
char* read_file(const char* filePath)
{
  FILE *file;

#ifdef _WIN32
  if(fopen_s(&file, filePath, "rb") == 0)
#else
  file = fopen(filePath, "rb");
  if(file)
#endif
  {
    size_t size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, sizeof(char), size, file);
    buffer[size] = '\0';

    fclose(file);

    return buffer;
  }
  else
  {
    SN_ASSERT(false, "Failed to Open the file: %s", filePath);
    return nullptr;
  }
}

// #############################################################################
//                           Vectors
// #############################################################################
Vec2 ivec2_f(const IVec2 &vec)
{
  return Vec2{
    (float)vec.x,
    (float)vec.y
  };
}

// #############################################################################
//                           Mat4
// #############################################################################
Mat4 projection_orthographic_matrix(float left, float right, float top, float bottom)
{
  Mat4 result = {0};

  result.ax = 2.0f / (right - left);
  result.by = 2.0f / (top - bottom);
  result.cz = -2.0f;
  result.dw = 1.0f;

  result.aw = -(right + left) / (right - left);
  result.bw = -(top + bottom) / (top - bottom);
  result.cw = 0.0f;
  
  return result;
}

Mat4 view_matrix(Vec2 pos, Vec2 size, float rot, int screenW, int screenH)
{
  Mat4 view = {0};
  float rad = DEG_2_RAD * rot;
  float cosV = cosf(rad);
  float sinV = sinf(rad);
  float zoomX = screenW / size.x;
  float zoomY = screenH / size.y;
  float tx = -pos.x;
  float ty = -pos.y;

  view.ax = cosV * zoomX;
  view.bx = -sinV * zoomX;
  view.ay = sinV * zoomY;
  view.by = cosV * zoomY;
  view.cz = 1.0f;
  view.dw = 1.0f;
  view.aw = (tx * cosV + ty * sinV) * zoomX + screenW * 0.5f;
  view.bw = (tx * (-sinV) + ty * cosV) * zoomY + screenH * 0.5f;

  return view;
}

// #############################################################################
//                           Math
// #############################################################################
void random_init_seed()
{
  srand(time(NULL));
}

int random_get_int(int min, int max)
{
  return (rand() % (max - min + 1)) + min;
}

float random_get_float(float max)
{
  return (float)rand() / (float)RAND_MAX;
}

// #############################################################################
//                           Collisions
// #############################################################################
bool collision_rects(const Rect &a, const Rect &b)
{
  return (a.pos.x < b.pos.x + b.size.x && a.pos.x + a.size.x > b.pos.x &&
          a.pos.y < b.pos.y + b.size.y && a.pos.y + a.size.y > b.pos.y);
}

bool collision_irects(const IRect &a, const IRect &b)
{
  return false;
}
