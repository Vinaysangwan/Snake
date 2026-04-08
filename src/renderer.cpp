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
Vec4 opengl_color(const Vec4 &color)
{
  return Vec4{
    color.r / 255.0f,
    color.g / 255.0f,
    color.b / 255.0f,
    color.a / 255.0f
  };
}

// #############################################################################
//                           Functions
// #############################################################################
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

// rendering
void render_sprite(SpriteID spriteID, Vec2 pos, float scale, int animIdx, Vec4 tintColor)
{
  if (renderState.transformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render Max Transform Count Reached: %d", renderState.transformCount);
  }
  
  const Sprite &sprite = SPRITES[spriteID];
  Vec2 size = {sprite.size.x * scale, sprite.size.y * scale};

  Transform transform = {
    .pos = pos,
    .size = size,
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(tintColor),
    .animIdx = animIdx,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.transforms[renderState.transformCount++] = transform;
}

void render_quad(Vec2 pos, Vec2 size, Vec4 color)
{
  if (renderState.transformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render Max Transform Count Reached: %d", renderState.transformCount);
  }
 
  const Sprite &sprite = SPRITES[SPRITE_QUAD];

  Transform transform = {
    .pos = pos,
    .size = {sprite.size.x * size.x, sprite.size.y * size.y},
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(color),
    .animIdx = 0,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.transforms[renderState.transformCount++] = transform;
}

void render_ui_sprite(SpriteID spriteID, Vec2 pos, float scale, int animIdx, Vec4 tintColor)
{ 
  if (renderState.uiTransformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render: Max Transform Count Reached: %d", renderState.uiTransformCount);
  }
 
  const Sprite &sprite = SPRITES[spriteID];
  Vec2 size = {sprite.size.x * scale, sprite.size.y * scale};

  Transform transform = {
    .pos = pos,
    .size = size,
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(tintColor),
    .animIdx = animIdx,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.uiTransforms[renderState.uiTransformCount++] = transform;
}

void render_ui_quad(Vec2 pos, Vec2 size, Vec4 color)
{
  if (renderState.uiTransformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render: Max Transform Count Reached: %d", renderState.uiTransformCount);
  }

  const Sprite &sprite = SPRITES[SPRITE_QUAD];

  Transform transform = {
    .pos = pos,
    .size = {sprite.size.x * size.x, sprite.size.y * size.y},
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(color),
    .animIdx = 0,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.uiTransforms[renderState.uiTransformCount++] = transform;
}
