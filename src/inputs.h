#pragma once
#include "pch.h"
#include "utils.h"

// #############################################################################
//                           keyboard
// #############################################################################
// Callbacks
void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// functions
bool key_down(int key);
bool key_up(int key);
bool key_pressed(int key);
bool key_released(int key);

// #############################################################################
//                           Mouse
// #############################################################################
// callbacks
void glfw_cursor_pos_callback(GLFWwindow* window, double xPos, double yPos);
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

// functions
float get_mouse_x();
float get_mouse_y();
Vec2 get_mouse_pos();
float get_delta_x();
float get_delta_y();
Vec2 get_mouse_delta();

bool mouse_button_down(int button);
bool mouse_button_up(int button);
bool mouse_button_pressed(int button);
bool mouse_button_released(int button);

float get_mouse_scroll_x();
float get_mouse_scroll_y();
Vec2 get_mouse_scroll();

// #############################################################################
//                           Common
// #############################################################################
void end_frame();
