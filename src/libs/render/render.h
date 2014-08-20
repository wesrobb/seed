#pragma once

#include <inttypes.h>
#include <stdbool.h>

/// Initializes the renderer with the specified width and height
/// and the specified vertex and fragment shaders. The atlas info
/// path and image path specify the image to load for all sprites
/// used by the renderer along with the data required to split it effectively.
bool render_init(uint32_t screen_width, uint32_t screen_height,
                 uint32_t virtual_width, uint32_t virtual_height,
                 const char* vert_shader_path, const char* frag_shader_path);

/// Clears the render command queue withut processing any commands.
void render_free(void);

/// Updates the viewport for rendering whenever the window is resized.
void render_resize(uint32_t screen_width, uint32_t screen_height);

/// Adds the sprite to the draw queue. If the queue is full or the sprite
/// uses a texture that is not the same as the previous sprite
/// then the queue is flushed (drawn) before adding the sprite.
void render_add_sprite(const struct sprite*);

/// Binds the texture to GL and updates the texture ID.
/// Returns false if texture load fails, true otherwise.
bool render_load_texture(struct texture*);

/// Deletes the texture object and unbinds it.
void render_delete_texture(struct texture*);

/// Prepares the renderer for drawing
void render_begin(void);

/// Draws all the sprites added to the renderer and then clears them.
void render_end(void);
