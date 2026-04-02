#pragma once

bool display_create(const char* title, int width, int height, bool isResizable = false);
void display_vsync(bool vsync);

void display_update();
void display_swap_buffers();

bool display_should_close();
void display_close();

float get_delta_time();
int get_fps();

void display_cleanup();
