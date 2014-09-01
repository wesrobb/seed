#pragma once

#include <inttypes.h>
#include <stdbool.h>

bool game_init(struct GLFWwindow* window,
               uint32_t virtual_width, uint32_t virtual_height);
// Updates the Game with the delta time since last update.
void game_update(double dt);
void game_cleanup(void);

void game_mouse_moved(double x_pos, double y_pos);
void game_mouse_pressed();

void game_key_pressed(int key);
void game_key_released(int key);
