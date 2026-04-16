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
  // cameras
  Camera gameCamera;
  Camera uiCamera;

  int fontHeight;
  Glyph glyphs[127];
  
  // transforms
  Transform transforms[MAX_TRANSFORM_COUNT];
  int transformCount = 0;

  Transform uiTransforms[MAX_TRANSFORM_COUNT];
  int uiTransformCount = 0;
};

// #############################################################################
//                           Statics
// #############################################################################
extern RenderState renderState;

// #############################################################################
//                           Functions
// #############################################################################
int animate(SpriteID spriteID, float &timer, float duration, float dt);

// rendering
void render_sprite(SpriteID spriteID, 
                   Vec2 pos = {0, 0}, 
                   float scale = 1.0f, 
                   int animIdx = 0, 
                   Vec4 tintColor = {255, 255, 255, 255});

void render_quad(Vec2 pos, Vec2 size, Vec4 color = {255, 255, 255, 255});

// ui rendering
void render_ui_sprite(SpriteID spriteID, 
                   Vec2 pos = {0, 0}, 
                   float scale = 1.0f, 
                   int animIdx = 0, 
                   Vec4 tintColor = {255, 255, 255, 255});

void render_ui_quad(Vec2 pos, Vec2 size, Vec4 color = {255, 255, 255, 255});
void render_ui_text(const char *text, Vec2 pos, float fontSize = 2, Vec4 color = {255, 255, 255, 255});
template<typename ...Args>
inline void render_ui_format_text(Vec2 pos, float fontSize, Vec4 color, const char* msg, Args ...args)
{
  render_ui_text(format_text(msg, args...), pos, fontSize, color);
}
