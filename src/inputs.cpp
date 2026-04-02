#include "pch.h"
#include "inputs.h"

// #############################################################################
//                           Constants
// #############################################################################
constexpr const int MAX_KEY_COUNT = GLFW_KEY_LAST;
constexpr const int MAX_MOUSE_BUTTON_COUNT = GLFW_MOUSE_BUTTON_LAST;

// #############################################################################
//                           Structs
// #############################################################################
// Keyboard
struct KeyboardState
{
  char currKeys[MAX_KEY_COUNT + 1] = {};  
  char prevKeys[MAX_KEY_COUNT + 1] = {};
};

// Mouse
struct MouseState
{
  char currButtons[MAX_MOUSE_BUTTON_COUNT + 1] = {};
  char prevButtons[MAX_MOUSE_BUTTON_COUNT + 1] = {};

  float x, y;
  float lastX, lastY;
  float scrollX, scrollY;
};

// #############################################################################
//                           Statics
// #############################################################################
static KeyboardState keyboard;

// #############################################################################
//                           keyboard
// #############################################################################
// Callbacks
void glfw_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    keyboard.currKeys[key] = true;
  }
  else if(action == GLFW_RELEASE)
  {
    keyboard.currKeys[key] = false;
  }
}

// functions
bool key_down(int key)
{
  return keyboard.currKeys[key];
}

bool key_up(int key)
{
  return !keyboard.currKeys[key];
}

bool key_pressed(int key)
{
  return !keyboard.prevKeys[key] && keyboard.currKeys[key];
}

bool key_released(int key)
{
  return keyboard.prevKeys[key] && !keyboard.currKeys[key];
}

// #############################################################################
//                           Mouse
// #############################################################################

// #############################################################################
//                           Common
// #############################################################################
void end_frame()
{
  // keyboard
  memcpy(&keyboard.prevKeys[0], &keyboard.currKeys[0], MAX_KEY_COUNT);
}
