#pragma once
#include "assets.h"

// #############################################################################
//                           Constants
// #############################################################################
constexpr const int MAX_TRANSFORM_COUNT = 1000;

// #############################################################################
//                           Structs
// #############################################################################
struct Transform
{
  Vec2 pos;
  Vec2 size;
  IVec2 atlasOffset;
  IVec2 spriteSize;
  int animIdx;
  int padding[3];
};

struct Camera
{
  Vec2 pos;
  Vec2 size;
  float rot = 0.0f;
};

struct RenderState
{
  Camera gameCamera;
  
  Transform transforms[MAX_TRANSFORM_COUNT];
  int transformCount = 0;
};

// #############################################################################
//                           Statics
// #############################################################################
extern RenderState renderState;

// #############################################################################
//                           Functions
// #############################################################################
void render_sprite(SpriteID spriteID, Vec2 pos = {0, 0}, float scale = 1.0f, int animIdx = 0);
int animate(SpriteID spriteID, float &timer, float duration, float dt);
