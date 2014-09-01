#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "khash.h"

#define SPRITE_ATLAS_NAME_LEN 64

KHASH_MAP_INIT_STR(sprite_map, int32_t);

// An atlas is a utility structure for reading texture rects
// from a texture atlas packed by TexturePacker using the
// AppGameKit template. 
// The atlas can be used to easily init sprites by name
// even though they are actually part of a texture atlas.

typedef struct atlas {
        struct rect* rect_sb;
        struct texture* texture;

        // Names for each of the sprites in the texture atlas.
        struct sprite_name {
                char name[SPRITE_ATLAS_NAME_LEN];
        };
        struct sprite_name* sprite_name_sb;

        // Hash map for getting sprites via name rather than ID.
        khash_t(sprite_map) *sprite_names_map;
} atlas;

// Initializes the atlas_info using the provided texture and
// atlas info. Does not take ownership of the texture.
// Returns false if initialization failed.
bool atlas_init(atlas*, struct texture*,
                const char* atlas_info_path);

// Resets the sprite altas to its default state.
void atlas_reset(atlas*);

// Inits a sprite from the atlas by name.
// Returns false if the sprite name is not found.
bool atlas_sprite_name(atlas*, struct sprite*,
                       const char* sprite_name,
                       float x_pos, float y_pos,
                       float x_anchor, float y_anchor,
                       float scale,
                       float rotation);

// Inits a sprite from the atlas by ID.
// Returns false the sprite ID is not found.
bool atlas_sprite_id(atlas*, struct sprite*,
                     int32_t sprite_id,
                     float x_pos, float y_pos,
                     float x_anchor, float y_anchor,
                     float scale,
                     float rotation);
