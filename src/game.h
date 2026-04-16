#pragma once
#include "assets.h"
#include "audio.h"

// #############################################################################
//                           Structs
// #############################################################################
struct Entity
{
  SpriteID spriteID;
  Vec2 pos;
  float speed;

  float timer = 0.0f;
  int animIdx = 0;
};

struct GameState
{
  // debug
  bool debug;
  
  // music
  Music *gameMusic;

  // sounds
  Sound *bounceSound;
  Sound *collectSound;

  // Score
  int score = 0;
  
  // entities
  Entity player;
  Entity bat;
};

// #############################################################################
//                           Statics
// #############################################################################
static GameState gameState;

// #############################################################################
//                           Functions
// #############################################################################
void game_init();
void game_update(float dt);
void game_render();
void game_cleanup();
