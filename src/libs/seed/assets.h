#pragma once

#include <stdbool.h>

// Initializes the assets singleton.
void assets_init();

// Resets the assets singleton to default state freeing
// any loaded assets in the process.
void assets_reset(struct renderer* r);

// Returns the texture for the specified path.
struct texture* assets_get_texture(const char* texture_path);

// Releases the specified texture back to the asset manager.
void assets_release_texture(struct texture* t, struct renderer* r);