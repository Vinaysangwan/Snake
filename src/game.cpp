#include "pch.h"
#include "game.h"
#include "renderer.h"
#include "inputs.h"
#include "config.h"

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
  }

  // Update Bat
  Entity &bat = gameState.bat;
  {
    static int direction = 1;
    if (bat.pos.x <= 10 || bat.pos.x >= WORLD_WIDTH - 10)
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
  Entity &player = gameState.player;
  render_sprite(player.spriteID, player.pos, 1.0f, player.animIdx);

  Entity &bat = gameState.bat;
  render_sprite(bat.spriteID, bat.pos, 1.0f, bat.animIdx);
}
