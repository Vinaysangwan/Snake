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
  gameState.playerID = SPRITE_SLIME;
  gameState.playerPos = {0, 0};
}

void game_update(float dt)
{
  static const float SPEED = 2.0f;
  
  if (key_down(GLFW_KEY_W)) gameState.playerPos.y -= SPEED;
  if (key_down(GLFW_KEY_S)) gameState.playerPos.y += SPEED;
  if (key_down(GLFW_KEY_A)) gameState.playerPos.x -= SPEED;
  if (key_down(GLFW_KEY_D)) gameState.playerPos.x += SPEED;
}

void game_render()
{
  render_sprite(gameState.playerID, gameState.playerPos, 2.0f);
}
