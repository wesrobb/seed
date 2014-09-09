#pragma once

#include <stdbool.h>

// Initializes the assets singleton.
bool assets_init();

// Resets the assets singleton to default state freeing
// any loaded assets in the process.
void assets_reset(struct renderer* r);

// Returns the texture for the specified path.
struct texture* assets_get_texture(const char* texture_path);
