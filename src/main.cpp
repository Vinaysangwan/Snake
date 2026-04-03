#include "pch.h"
#include "utils.h"
#include "display.h"
#include "inputs.h"
#include "gl.h"

int main(void)
{
  // Init display
  if(!display_create("Snake", 1280, 720))
  {
    return -1;
  }

  // Init gl
  if(!gl_init())
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

    gl_render();

    display_swap_buffers();
  }

  // cleanups
  gl_cleanup();
  display_cleanup();
}
