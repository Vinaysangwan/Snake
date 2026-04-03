#include "pch.h"
#include "display.h"
#include "utils.h"
#include "inputs.h"

// #############################################################################
//                           Statics
// #############################################################################
static GLFWwindow* window = nullptr;
static double lastFrameTime = 0.0;
static float deltaTime = 0.0f;
static float fpsTimer = 0.0f;
static int fpsCounter = 0;
static int FPS = 0;

// #############################################################################
//                           Functions
// #############################################################################
bool display_create(const char *title, int width, int height, bool isResizable)
{
  // Init glfw
  if(!glfwInit())
  {
    SN_ASSERT(false, "Failed to Init GLFW!");
    return false;
  }

  // window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, isResizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // Window Creation
  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window)
  {
    SN_ASSERT(false, "Failed to Create Window!");
    return false;
  }
  glfwMakeContextCurrent(window);

  // glfw callbacks
  glfwSetKeyCallback(window, glfw_key_callback);
  glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
  glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
  glfwSetScrollCallback(window, glfw_scroll_callback);

  // Init glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    SN_ASSERT(false, "Failed to init Glad");
    return false;
  }

  // gl viewport
  glViewport(0, 0, width, height);

  // gl Enables
  

  // Show Window
  glfwShowWindow(window);

  // Init lastFrameTime
  lastFrameTime = glfwGetTime();

  return true;
}

void display_vsync(bool vsync)
{
  glfwSwapInterval(vsync ? GLFW_TRUE : GLFW_FALSE);
}

void display_update()
{
  // Calculate Delta time
  float currentFrameTime = glfwGetTime();
  deltaTime = (float)(currentFrameTime - lastFrameTime);
  lastFrameTime = currentFrameTime;

  // calculate FPS
  fpsTimer += deltaTime;
  fpsCounter++;
  if (fpsTimer >= 1.0f)
  {
    fpsTimer -= 1.0f;
    FPS = fpsCounter;
    fpsCounter = 0;
  }
  
  glfwPollEvents();
}

void display_swap_buffers()
{
  glfwSwapBuffers(window);

  end_frame();
}

bool display_should_close()
{
  return glfwWindowShouldClose(window);
}

void display_close()
{
  glfwSetWindowShouldClose(window, true);
}

float get_delta_time()
{
  return deltaTime;
}

int get_fps()
{
  return FPS;
}

void display_cleanup()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}
