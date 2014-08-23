#pragma once

#include <stdbool.h>

#include <render/spritebatch.h>

#define LAYER_MAX_LAYERS 5
#define LAYER_MAX_WIDTH 200
#define LAYER_MAX_HEIGHT 200

typedef struct layer {
        short tile[LAYER_MAX_WIDTH][LAYER_MAX_HEIGHT];
} layer;

typedef struct tilemap {
        int tiles_wide;
        int tiles_high;
        int tile_width;
        int tile_height;
        int layer_count;
        layer layers[LAYER_MAX_LAYERS];
        //spritebatch spritebatch;
} tilemap;

// Initializes the specified tilemap from the specified pyxel map file,
// sprite atlas info, and sprite sheet.
// Returns true if initialization was successful, false otherwise.
bool tilemap_init(tilemap*, const char* map_file, 
                  const char* atlas_file, const char* sprite_sheet_file);
                  
// Draws the tile map.
void tilemap_draw(tilemap*);