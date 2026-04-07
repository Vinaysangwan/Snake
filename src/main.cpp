#include "pch.h"
#include "utils.h"
#include "display.h"
#include "inputs.h"
#include "gl.h"
#include "game.h"
#include "config.h"
#include "audio.h"
#include "renderer.h"

// #############################################################################
//                           Constants
// #############################################################################
constexpr const int TARGET_FPS = 60;
constexpr const float TARGET_DELTA_TIME = 1.0f / TARGET_FPS;

// #############################################################################
//                           Functions
// #############################################################################
int main(void)
{
  // Init display
  if(!display_create("Snake", WINDOW_WIDTH, WINDOW_HEIGHT))
  {
    return -1;
  }
  display_vsync(true);

  // Init audio
  if (!audio_init())
  {
    return -1;
  }

  // Init random seed
  random_init_seed();

  // size of transform
  SN_INFO("Size of Transform: %zu", sizeof(Transform));

  // Init gl
  if(!gl_init())
  {
    return -1;
  }

  // init game
  game_init();

  // Main Game Loop
  while(!display_should_close())
  {
    display_update();

    float dt = get_delta_time();

    // Print FPS
    if (false)
    {
      static float timer = 0;
      timer += dt;
      if (timer >= 1.0f)
      {
        timer = 0;
        SN_INFO("FPS: %d", get_fps());
      }
    }

    // TODO: Remove this when creating menu
    if (key_down(GLFW_KEY_ESCAPE))
    {
      display_close();
    }

    // Update Loop
    static float updateLoopTimer = 0;
    updateLoopTimer += dt;
    while(updateLoopTimer >= TARGET_DELTA_TIME)
    {
      updateLoopTimer -= TARGET_DELTA_TIME;

      game_update(TARGET_DELTA_TIME);
    }

    // rendering
    game_render();
    gl_render();
    
    // buffer swap
    display_swap_buffers();
  }

  // cleanups
  game_cleanup();
  gl_cleanup();
  audio_close();
  display_cleanup();
}
