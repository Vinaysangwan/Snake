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
Vec2 vec2i_f(IVec2 vec)
{
  return Vec2(
    (float)vec.x,
    (float)vec.y
  );
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
