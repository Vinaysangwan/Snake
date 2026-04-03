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
void render_sprite(SpriteID spriteID, Vec2 pos, float scale)
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
    .spriteSize = sprite.size
  };

  renderState.transforms[renderState.transformCount++] = transform;
}
