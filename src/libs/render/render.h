#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include <kazmath/kazmath.h>

#include "sprite.h"

/// Initializes the renderer with the specified width and height
/// and the specified vertex and fragment shaders. The atlas info
/// path and image path specify the image to load for all sprites
/// used by the renderer along with the data required to split it effectively.
bool Render_init(uint32_t screen_width, uint32_t screen_height,
                 uint32_t virtual_width, uint32_t virtual_height,
                 const char* vert_shader_path, const char* frag_shader_path);

/// Clears the render command queue withut processing any commands.
void Render_free(void);

/// Updates the viewport for rendering whenever the window is resized.
void Render_resize(uint32_t screen_width, uint32_t screen_height);

/// Adds the sprite to the draw queue. If the queue is full or the sprite
/// uses a texture that is not the same as the previous sprite
/// then the queue is flushed (drawn) before adding the sprite.
void Render_addSprite(const Sprite*);

/// Binds the texture to GL and returns the GL texture object.
/// Returns -1 if texture load fails.
uint32_t Render_loadTexture(unsigned char* data,
                            uint32_t width, uint32_t height,
                            uint32_t channels);

/// Prepares the renderer for drawing
void Render_begin(void);

/// Draws all the sprites added to the renderer and then clears them.
void Render_end(void);
