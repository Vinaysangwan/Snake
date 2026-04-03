#include "pch.h"
#include "renderer.h"
#include "utils.h"

// #############################################################################
//                           Statics
// #############################################################################
RenderState renderState = {};

// #############################################################################
//                           Functions
// #############################################################################
void render_sprite(SpriteID spriteID, Vec2 pos, float scale, int animIdx)
{
  if (renderState.transformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render Max Transform Count Reached: %d", renderState.transformCount);
  }
  
  Sprite sprite = SPRITES[spriteID];
  Vec2 size = {sprite.size.x * scale, sprite.size.y * scale};

  Transform transform = {
    .pos = {pos.x - size.x / 2, pos.y - size.y / 2},
    .size = size,
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .animIdx = animIdx
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
