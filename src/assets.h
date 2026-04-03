#pragma once
#include "utils.h"

// #############################################################################
//                           Sprites
// #############################################################################
enum SpriteID
{
  SPRITE_SLIME,
  SPRITE_BAT,

  SPRITE_COUNT
};

struct Sprite
{
  IVec2 atlasOffset;
  IVec2 size;
  int frameCount = 1;
};

static const Sprite SPRITES[SPRITE_COUNT] = {
  [SPRITE_SLIME] = {.atlasOffset = {0, 0}, .size = {20, 20}, .frameCount = 2},
  [SPRITE_BAT] = {.atlasOffset = {0, 20}, .size = {20, 20}, .frameCount = 4},
};
