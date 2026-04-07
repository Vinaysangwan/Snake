#ifdef ENGINE
// Engine Code
#pragma once

#include <utils.h>

using vec2 = Vec2;
using ivec2 = IVec2;
using vec4 = Vec4;

#else
#define BIT(x) (1 << (x))

#endif

// #############################################################################
//                           Constants
// #############################################################################
// rendering options
#define RENDERING_OPTION_SPRITE BIT(0)
#define RENDERING_OPTION_FONT BIT(1)

// #############################################################################
//                           Structs
// #############################################################################
struct Transform
{
  vec2 pos;
  vec2 size;
  ivec2 atlasOffset;
  ivec2 spriteSize;
  vec4 tintColor;
};
