#include "pch.h"
#include "game.h"
#include "renderer.h"
#include "inputs.h"
#include "config.h"

// #############################################################################
//                           Constants
// #############################################################################
constexpr const float GRID_SIZE = 20;
constexpr const int MAP_ROWS = 9;
constexpr const int MAP_COLS = 16;
constexpr const int MAP_SIZE = MAP_ROWS * MAP_COLS;

constexpr const float LEFT_WALL = GRID_SIZE;
constexpr const float RIGHT_WALL = WORLD_WIDTH - GRID_SIZE;
constexpr const float TOP_WALL = 2.0f * GRID_SIZE;
constexpr const float BOTTOM_WALL = WORLD_HEIGHT - GRID_SIZE;

constexpr const int MAP[MAP_SIZE] = {
   0,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  3, 
   4,  5,  5,  5,  6,  5,  5,  5,  5,  5,  5,  5,  6,  5,  5,  7, 
   8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 11, 
   4,  9,  9,  9,  9,  9,  9, 10,  9,  9,  9,  9, 10,  9,  9,  7, 
   8,  9,  9, 10,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 11, 
   4,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  7, 
   8,  9,  9,  9,  9,  9,  9,  9,  9, 10,  9,  9,  9,  9,  9, 11, 
   4,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  7, 
  12, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 15, 
};

// #############################################################################
//                           Functions
// #############################################################################
void game_init()
{
  // Init game Camera
  {
    Camera &gameCamera = renderState.gameCamera;
    gameCamera.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
    gameCamera.size = {WORLD_WIDTH, WORLD_HEIGHT};
  }
  
  // Init Player Data
  {
    Entity &player = gameState.player;
    player.spriteID = SPRITE_SLIME;
    player.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
  }

  // Init Bat Data
  {
    Entity &bat = gameState.bat;
    bat.spriteID = SPRITE_BAT;
    bat.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
  }
}

void game_update(float dt)
{
  static const float SPEED = 1.0f;
  
  // Update Player
  Entity &player = gameState.player;
  {
    if (key_down(GLFW_KEY_W)) player.pos.y -= SPEED;
    if (key_down(GLFW_KEY_S)) player.pos.y += SPEED;
    if (key_down(GLFW_KEY_A)) player.pos.x -= SPEED;
    if (key_down(GLFW_KEY_D)) player.pos.x += SPEED;

    player.pos.x = clamp(player.pos.x, LEFT_WALL + 10, RIGHT_WALL - 10);
    player.pos.y = clamp(player.pos.y, TOP_WALL + 10, BOTTOM_WALL - 10);
  }

  // Update Bat
  Entity &bat = gameState.bat;
  {
    static int direction = 1;
    if (bat.pos.x <= LEFT_WALL + 10 || bat.pos.x >= RIGHT_WALL - 10)
    {
      direction *= -1;
    }
    bat.pos.x += direction * SPEED;
  }

  player.animIdx = animate(player.spriteID, player.timer, 1.0f, dt);
  bat.animIdx = animate(bat.spriteID, bat.timer, 0.8f, dt);
}

void game_render()
{
  // render map
  for (int i=0; i < MAP_SIZE; ++i)
  {
    int r = i / MAP_COLS;
    int c = i % MAP_COLS;
    
    render_sprite((SpriteID)((int)SPRITE_WALL_00 + MAP[i]), {c * GRID_SIZE + 10, r * GRID_SIZE + 10});
  }

  // render bat
  Entity &bat = gameState.bat;
  render_sprite(bat.spriteID, bat.pos, 1.0f, bat.animIdx);

  // render player
  Entity &player = gameState.player;
  render_sprite(player.spriteID, player.pos, 1.0f, player.animIdx);
}
