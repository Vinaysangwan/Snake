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

    if (key_down(GLFW_KEY_ESCAPE))
    {
      display_close();
    }

    display_swap_buffers();
  }

  // cleanups
  display_cleanup();
}
