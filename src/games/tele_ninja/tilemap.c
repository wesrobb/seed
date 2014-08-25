#include "tilemap.h"

#include <assert.h>
#include <stdio.h>

#include <core/stretchy_buffer.h>
#include <log/log.h>
#include <render/atlas.h>
#include <render/sprite.h>

bool parse_map_file(tilemap* tm, const char* map_file);
void update_sprites(tilemap* tm);

bool tilemap_init(tilemap* tm, atlas* atlas, const char* map_file)
{
        assert(tm);

        memset(tm, 0, sizeof(*tm));
        tm->atlas = atlas;
        if (!parse_map_file(tm, map_file)) {
                return false;
        }

        update_sprites(tm);

        return true;
}

void tilemap_reset(tilemap* tm)
{
        assert(tm);

        sb_free(tm->layer_sb);
        sb_free(tm->sprite_sb);
        memset(tm, 0, sizeof(*tm));
}

bool parse_map_file(tilemap* tm, const char* map_file)
{
        FILE* map = fopen(map_file, "r");
        if (!map) {
                LOGERR("Failed to open map file %s", map_file);
                return false;
        }

        // Read header
        fscanf(map, "tileswide %d\n", &tm->tiles_wide);
        fscanf(map, "tileshigh %d\n", &tm->tiles_high);
        fscanf(map, "tilewidth %d\n", &tm->tile_width);
        fscanf(map, "tileheight %d\n", &tm->tile_height);

        while (!feof(map)) {
                int layer_index = -1;
                fscanf(map, "\nlayer %d\n", &layer_index);

                // No more layers.
                if (layer_index == -1) {
                        break;
                }

                layer* l = sb_add(tm->layer_sb, 1);
                l->tile_sb = NULL;
                int16_t* tile = sb_add(l->tile_sb, 
                                       (int)(tm->tiles_high * tm->tiles_wide));
                for (int32_t i = tm->tiles_high - 1; i >= 0; i--) {
                        for (int32_t j = 0; j < tm->tiles_wide; ++j) {
                                fscanf(map, "%hd,", &tile[tm->tiles_wide * i + j]);
                        }
                }
        }

        return true;
}

void update_sprites(tilemap* tm)
{
        for (int32_t h = 0; h < sb_count(tm->layer_sb); ++h) {
                for (int32_t i = 0; i < tm->tiles_high; ++i) {
                        for (int32_t j = 0; j < tm->tiles_wide; ++j) {
                                int16_t tile = tm->layer_sb[h].tile_sb[tm->tiles_wide * i + j];
                                if (tile == -1) {
                                        continue;
                                }
                                sprite* s = sb_add(tm->sprite_sb, 1);
                                atlas_sprite_id(tm->atlas, s, tile,
                                                (float)j * tm->tile_width,
                                                (float)i * tm->tile_height,
                                                0, 0,
                                                (float)tm->tile_width,
                                                (float)tm->tile_height,
                                                0);
                        }
                }
        }
}