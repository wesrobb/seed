#pragma once

typedef struct assets assets;

// Returns the one and only handle for loading assets.
assets* assets_get();

struct texture* assets_get_texture(const char* texture_path);