#pragma once
#include "utils.h"

// #############################################################################
//                           Sprites
// #############################################################################
enum SpriteID
{
  SPRITE_SLIME,

  SPRITE_COUNT
};

struct Sprite
{
  IVec2 atlasOffset;
  IVec2 size;
};

static const Sprite SPRITES[SPRITE_COUNT] = {
  [SPRITE_SLIME] = {.atlasOffset = {0, 0}, .size = {16, 16}},
};
