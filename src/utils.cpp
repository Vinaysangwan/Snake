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
