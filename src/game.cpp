#include "pch.h"
#include "game.h"
#include "renderer.h"
#include "inputs.h"

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
  if (key_down(GLFW_KEY_W)) gameState.playerPos.y -= 0.02f;
  if (key_down(GLFW_KEY_S)) gameState.playerPos.y += 0.02f;
  if (key_down(GLFW_KEY_A)) gameState.playerPos.x -= 0.02f;
  if (key_down(GLFW_KEY_D)) gameState.playerPos.x += 0.02f;
}

void game_render()
{
  render_sprite(gameState.playerID, gameState.playerPos, 1/32.0f);
}
