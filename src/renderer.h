#pragma once
#include "assets.h"
#include "shared_header.h"

// #############################################################################
//                           Constants
// #############################################################################
constexpr const int MAX_TRANSFORM_COUNT = 1000;

// #############################################################################
//                           Structs
// #############################################################################
struct Glyph
{
  Vec2 offset;
  Vec2 advance;
  IVec2 textureCoords;
  IVec2 size;
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

  int fontHeight;
  Glyph glyphs[127];
  
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
void render_sprite(SpriteID spriteID, Vec2 pos = {0, 0}, float scale = 1.0f, int animIdx = 0, Vec3 tintColor = {255, 255, 255});
void render_quad(Vec2 pos, Vec2 size, Vec3 color = {255, 255, 255});
int animate(SpriteID spriteID, float &timer, float duration, float dt);
