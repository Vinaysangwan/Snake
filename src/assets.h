#pragma once
#include "utils.h"

// #############################################################################
//                           Sprites
// #############################################################################
enum SpriteID
{
  SPRITE_QUAD,
  SPRITE_SLIME,
  SPRITE_BAT,

  SPRITE_WALL_00,
  SPRITE_WALL_01,
  SPRITE_WALL_02,
  SPRITE_WALL_03,
  SPRITE_WALL_10,
  SPRITE_WALL_11,
  SPRITE_WALL_12,
  SPRITE_WALL_13,
  SPRITE_WALL_20,
  SPRITE_WALL_21,
  SPRITE_WALL_22,
  SPRITE_WALL_23,
  SPRITE_WALL_30,
  SPRITE_WALL_31,
  SPRITE_WALL_32,
  SPRITE_WALL_33,

  SPRITE_COUNT
};

struct Sprite
{
  IVec2 atlasOffset;
  IVec2 size;
  IVec2 colliderOffset = IVec2{};
  IVec2 colliderSize = size;
  int frameCount = 1;
};

inline constexpr const Sprite SPRITES[SPRITE_COUNT] = {
  [SPRITE_QUAD] = {.atlasOffset = {0, 40}, .size = {1, 1}},
  
  [SPRITE_SLIME] = {.atlasOffset = {0, 0}, .size = {20, 20}, .colliderOffset = {2, 4}, .colliderSize = {15, 12}, .frameCount = 2},
  [SPRITE_BAT] = {.atlasOffset = {0, 20}, .size = {20, 20}, .colliderOffset = {6, 5}, .colliderSize = {7, 6}, .frameCount = 4},

  [SPRITE_WALL_00] = {.atlasOffset = { 0, 100}, .size = {20, 20}},
  [SPRITE_WALL_01] = {.atlasOffset = {20, 100}, .size = {20, 20}},
  [SPRITE_WALL_02] = {.atlasOffset = {40, 100}, .size = {20, 20}},
  [SPRITE_WALL_03] = {.atlasOffset = {60, 100}, .size = {20, 20}},
  [SPRITE_WALL_10] = {.atlasOffset = { 0, 120}, .size = {20, 20}},
  [SPRITE_WALL_11] = {.atlasOffset = {20, 120}, .size = {20, 20}},
  [SPRITE_WALL_12] = {.atlasOffset = {40, 120}, .size = {20, 20}},
  [SPRITE_WALL_13] = {.atlasOffset = {60, 120}, .size = {20, 20}},
  [SPRITE_WALL_20] = {.atlasOffset = { 0, 140}, .size = {20, 20}},
  [SPRITE_WALL_21] = {.atlasOffset = {20, 140}, .size = {20, 20}},
  [SPRITE_WALL_22] = {.atlasOffset = {40, 140}, .size = {20, 20}},
  [SPRITE_WALL_23] = {.atlasOffset = {60, 140}, .size = {20, 20}},
  [SPRITE_WALL_30] = {.atlasOffset = { 0, 160}, .size = {20, 20}},
  [SPRITE_WALL_31] = {.atlasOffset = {20, 160}, .size = {20, 20}},
  [SPRITE_WALL_32] = {.atlasOffset = {40, 160}, .size = {20, 20}},
  [SPRITE_WALL_33] = {.atlasOffset = {60, 160}, .size = {20, 20}},
};
