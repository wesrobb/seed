#pragma once

#include <inttypes.h>
#include <stdbool.h>

bool Game_init(struct GLFWwindow* window,
               uint32_t virtual_width, uint32_t virtual_height);
// Updates the Game with the delta time since last update.
void Game_update(double dt);
void Game_render(double interpolation);
void Game_cleanup(void);

void Game_mouseMoved(double x_pos, double y_pos);
void Game_mousePressed();
