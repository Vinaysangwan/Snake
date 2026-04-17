#pragma once
#include "assets.h"
#include "audio.h"

// #############################################################################
//                           Constants
// #############################################################################
constexpr const int MAX_SNAKE_SECTIONS = 84;

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

struct SnakeSection
{
  Vec2 pos;
  Vec2 prevPos;
};

struct Snake
{
  SpriteID spriteID;

  float speed;
  Vec2 direction;
  Vec2 nextDirection;
  Vec2 destination;

  SnakeSection sections[MAX_SNAKE_SECTIONS];
  int sectionCount = 0;

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
  Snake player;
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
