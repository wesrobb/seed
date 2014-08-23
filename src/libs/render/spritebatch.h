//#pragma once
//
//#include <inttypes.h>
//#include <stdbool.h>
//
//#include "sprite.h"
//
//
//#include "khash.h"
//#include "texture.h"
//
//#define SPRITEBATCH_MAX_SPRITES 5000
//#define SPRITEBATCH_MAX_ATLAS_IMAGES 100
//#define SPRITEBATCH_MAX_SPRITE_NAME_LEN 64
//
//KHASH_MAP_INIT_STR(sp, uint32_t);
//
//// A sprite batch is used for rendering a large number of sprites from
//// a single texture using a single GL draw call.
//typedef struct spritebatch {
//        sprite sprites[SPRITEBATCH_MAX_SPRITES];
//
//        // List of active/inactive sprites in the sprites array.
//        sprite *active_sprites, *inactive_sprites;
//
//        texture texture;
//
//        // The number of images in the texture atlas.
//        uint32_t num_images;
//
//        // The texture coordinates for each sprite in the texture atlas.
//        float tex_coords[SPRITEBATCH_MAX_ATLAS_IMAGES * 8];
//
//        // Names for each of the sprites in the texture atlas.
//        char sprite_names[SPRITEBATCH_MAX_ATLAS_IMAGES *
//                          SPRITEBATCH_MAX_SPRITE_NAME_LEN];
//
//        // Hash map for adding sprites via name rather than ID.
//        khash_t(sp) *sprite_names_map;
//} spritebatch;
//
//// Initializes the sprite batch using the provided texture atlas info and
//// texture atlas. Returns false if initialization failed.
//bool spritebatch_init(spritebatch*,
//                      const char* atlas_info_path,
//                      const char* texture_atlas_path);
//
//// Releases the texture data stored in the spritebatch.
//void spritebatch_destroy(spritebatch*);
//
//// Sends all the sprites in the specified spritebatch to the renderer.
//void spritebatch_draw(const spritebatch*);
//
//// Adds a sprite to the sprite batch and returns a pointer to it.
//// Ownership is not transferred.
//// Returns NULL if the spritebatch is full or the sprite name is not found.
//sprite* spritebatch_add_sprite_name(spritebatch*,
//                                   const char* sprite_name,
//                                   float x_pos, float y_pos,
//                                   float x_anchor, float y_anchor,
//                                   float width, float height,
//                                   float rotation);
//
//// Adds a sprite to the sprite batch and returns a pointer to it.
//// Ownership is not transferred.
//// Returns NULL if the spritebatch is full or the sprite name is not found.
//sprite* spritebatch_add_sprite_id(spritebatch*,
//                                  uint32_t sprite_id,
//                                  float x_pos, float y_pos,
//                                  float x_anchor, float y_anchor,
//                                  float width, float height,
//                                  float rotation);
//
//// Removes the specified sprite from the sprite batch and
//// sets the sprite pointer to NULL.
//void spritebatch_remove_sprite(spritebatch*, sprite**);
//
//// Clears all sprites in the sprite batch.
//void spritebatch_clear_sprites(spritebatch*);
