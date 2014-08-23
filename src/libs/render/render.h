#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct renderer renderer;

// Creates a renderer with the on the specified GLFW window
// and the specified vertex and fragment shaders.
// Returns null if renderer creation fails.
renderer* render_create(struct GLFWwindow* window,
                        uint32_t virtual_width, uint32_t virtual_height,
                        const char* vert_shader_path, const char* frag_shader_path);

// Stops rendering and frees the renderer.
void render_free(renderer*);

// Updates the viewport for rendering whenever the window is resized.
void render_resize(renderer*, uint32_t screen_width, uint32_t screen_height);

// Adds the sprite to the renderer for drawing at the next render_submit call.
// Note that the sprite data is copied into the renderer.
void render_add_sprite(renderer*, const struct sprite*);
// Adds the sprites to the renderer for drawing at the next render_submit call.
// Note that each sprite's data is copied into the renderer.
void render_add_sprites(renderer*, const struct sprite*, int32_t sprites_len);

// Deletes the texture object and unbinds it.
void render_delete_texture(renderer*, struct texture*);

// Draws all the sprites added to the renderer and then removes them.
void render_submit(renderer*);
