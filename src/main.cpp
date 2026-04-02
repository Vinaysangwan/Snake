#include "pch.h"
#include "utils.h"
#include "display.h"
#include "inputs.h"

int main(void)
{
  // Init display
  if(!display_create("Snake", 1280, 720))
  {
    return -1;
  }

  // Main Game Loop
  while(!display_should_close())
  {
    display_update();

    float dt = get_delta_time();
    static float timer = 0;
    timer += dt;
    if (timer >= 1.0f)
    {
      SN_INFO("FPS: %d", get_fps());
      timer = 0;
    }

    display_swap_buffers();
  }

  // cleanups
  display_cleanup();
}
