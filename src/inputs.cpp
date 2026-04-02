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
  double x, y;
  double lastX, lastY;

  char currButtons[MAX_MOUSE_BUTTON_COUNT + 1] = {};
  char prevButtons[MAX_MOUSE_BUTTON_COUNT + 1] = {};

  double scrollX, scrollY;
};

// #############################################################################
//                           Statics
// #############################################################################
static KeyboardState keyboard;
static MouseState mouse;

// #############################################################################
//                           keyboard
// #############################################################################
// callbacks
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
// callbacks
void glfw_cursor_pos_callback(GLFWwindow *window, double xPos, double yPos)
{
  mouse.lastX = mouse.x;
  mouse.lastY = mouse.y;

  mouse.x = xPos;
  mouse.y = yPos;
}

void glfw_mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    mouse.currButtons[button] = true;
  }
  else if(action == GLFW_RELEASE)
  {
    mouse.currButtons[button] = false;
  }
}

void glfw_scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
  mouse.scrollX = xOffset;
  mouse.scrollY = yOffset;
}

// functions
float get_mouse_x()
{
  return (float)mouse.x;
}

float get_mouse_y()
{
  return (float)mouse.y;
}

Vec2 get_mouse_pos()
{
  return Vec2((float)mouse.x, (float)mouse.y);
}

float get_delta_x()
{
  return (float)(mouse.x - mouse.lastX);
}

float get_delta_y()
{
  return (float)(mouse.y - mouse.lastY);
}

Vec2 get_mouse_delta()
{
  return Vec2(
    (float)(mouse.x - mouse.lastX),
    (float)(mouse.y - mouse.lastY)
  );
}

bool mouse_button_down(int button)
{
  return mouse.currButtons[button];
}

bool mouse_button_up(int button)
{
  return !mouse.currButtons[button];
}

bool mouse_button_pressed(int button)
{
  return !mouse.prevButtons[button] && mouse.currButtons[button];
}

bool mouse_button_released(int button)
{
  return mouse.prevButtons[button] && !mouse.currButtons[button];
}

float get_mouse_scroll_x()
{
  return (float)mouse.scrollX;  
}

float get_mouse_scroll_y()
{
  return (float)mouse.scrollY;
}

Vec2 get_mouse_scroll()
{
  return Vec2(
    (float)mouse.scrollX,
    (float)mouse.scrollY
  );
}

// #############################################################################
//                           Common
// #############################################################################
void end_frame()
{
  // keyboard
  memcpy(&keyboard.prevKeys[0], &keyboard.currKeys[0], MAX_KEY_COUNT);

  // mouse
  memcpy(&mouse.prevButtons[0], &mouse.currButtons[0], MAX_MOUSE_BUTTON_COUNT);
  mouse.scrollX = 0;
  mouse.scrollY = 0;
}
