#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct layer {
        int16_t* tile_sb;
} layer;

typedef struct tilemap {
        int32_t tiles_wide;
        int32_t tiles_high;
        int32_t tile_width;
        int32_t tile_height;
        layer* layer_sb;
        struct atlas* atlas;
        struct sprite* sprite_sb;
} tilemap;

// Initializes the specified tilemap from the specified pyxel map file
// and atlas. This does not take ownership of the atlas.
// Returns true if initialization was successful, false otherwise.
bool tilemap_init(tilemap*, struct atlas*, const char* map_file);

// Frees any allocation done by the tilemap and resets it to the default state.
void tilemap_reset(tilemap*);