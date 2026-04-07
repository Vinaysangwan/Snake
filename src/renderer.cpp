#include "pch.h"
#include "renderer.h"
#include "utils.h"

// #############################################################################
//                           Statics
// #############################################################################
RenderState renderState = {};

// #############################################################################
//                           internal functions
// #############################################################################
Vec3 opengl_color(const Vec3 &color)
{
  return Vec3{
    color.r / 255.0f,
    color.g / 255.0f,
    color.b / 255.0f
  };
}

// #############################################################################
//                           Functions
// #############################################################################
void render_sprite(SpriteID spriteID, Vec2 pos, float scale, int animIdx, Vec3 tintColor)
{
  if (renderState.transformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render Max Transform Count Reached: %d", renderState.transformCount);
  }
  
  const Sprite &sprite = SPRITES[spriteID];
  Vec2 size = {sprite.size.x * scale, sprite.size.y * scale};

  const Vec3 &color = opengl_color(tintColor);

  Transform transform = {
    .pos = pos,
    .size = size,
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = Vec4{color.r, color.g, color.b, (float)animIdx}
  };

  renderState.transforms[renderState.transformCount++] = transform;
}

void render_quad(Vec2 pos, Vec2 size, Vec3 color)
{
  const Sprite &sprite = SPRITES[SPRITE_QUAD];

  const Vec3 &op_color = opengl_color(color);

  Transform transform = {
    .pos = pos,
    .size = {sprite.size.x * size.x, sprite.size.y * size.y},
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = Vec4{op_color.r, op_color.g, op_color.b, 0}
  };

  renderState.transforms[renderState.transformCount++] = transform;
}

int animate(SpriteID spriteID, float &timer, float duration, float dt)
{
  Sprite sprite = SPRITES[spriteID];
  timer += dt;
  while (timer > duration)
  {
    timer -= duration;
  }

  return (int)(sprite.frameCount * timer / duration);
}
