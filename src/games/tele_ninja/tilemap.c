#include "tilemap.h"

#include <assert.h>
#include <stdio.h>

#include <seed/atlas.h>
#include <seed/log.h>
#include <seed/parson.h>
#include <seed/sprite.h>
#include <seed/stretchy_buffer.h>

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
        JSON_Value* root = json_parse_file(map_file);
        if (!root) {
                LOGERR("Failed to parse json from map file %s", map_file);
                return false;
        }
        JSON_Object* root_obj = json_value_get_object(root);
        tm->tiles_wide = (int32_t)json_object_get_number(root_obj, "tileswide");
        tm->tiles_high = (int32_t)json_object_get_number(root_obj, "tileshigh");
        tm->tile_width = (int32_t)json_object_get_number(root_obj, "tilewidth");
        tm->tile_height = (int32_t)json_object_get_number(root_obj, "tileheight");

        JSON_Array* layer_arr = json_object_get_array(root_obj, "layers");
        for (size_t i = 0; i < json_array_get_count(layer_arr); ++i) {
                layer* l = sb_add(tm->layer_sb, 1);
                memset(l, 0, sizeof(*l));

                JSON_Object* layer_obj = json_array_get_object(layer_arr, i);
                l->index = (int16_t)json_object_get_number(layer_obj, "number");
                strcpy(l->name, json_object_get_string(layer_obj, "name"));

                JSON_Array* tile_arr = json_object_get_array(layer_obj, "tiles");
                size_t tile_count = json_array_get_count(tile_arr);
                tile* tile_start = sb_add(l->tile_sb, (int)tile_count);
                for (size_t j = 0; j < tile_count; ++j) {
                        JSON_Object* tile_object = json_array_get_object(tile_arr, j);
                        tile* t = &tile_start[j];
                        t->value = (int16_t)json_object_get_number(tile_object, "tile");
                        t->flip_x = (bool)json_object_get_boolean(tile_object, "flipX");
                        t->pos.x = (float)json_object_get_number(tile_object, "x");
                        t->pos.y = (float)json_object_get_number(tile_object, "y");
                        t->rot = (float)json_object_get_number(tile_object, "rot");
                }
        }

        json_value_free(root);
        return true;
}

void update_sprites(tilemap* tm)
{
        for (int32_t h = 0; h < sb_count(tm->layer_sb); ++h) {
                for (int32_t i = 0; i < tm->tiles_high; ++i) {
                        for (int32_t j = 0; j < tm->tiles_wide; ++j) {
                                layer* l = &tm->layer_sb[h];
                                tile* t = &l->tile_sb[tm->tiles_wide * i + j];
                                if (t->value == -1) {
                                        continue;
                                }
                                sprite* s = sb_add(tm->sprite_sb, 1);
                                s->flip_x = t->flip_x;

                                // PyxelEdit's y index is backwards.
                                float y_index = tm->tiles_high - t->pos.y - 1;

                                float x_pos = t->pos.x * tm->tile_width;
                                float y_pos = y_index * tm->tile_height;
                                atlas_sprite_id(tm->atlas, s, t->value,
                                                x_pos, y_pos,
                                                x_pos + (tm->tile_width / 2.0f), 
                                                y_pos + (tm->tile_height / 2.0f),
                                                1.0f,
                                                t->rot * -90); // 0 = 0, 1 = 90, 2 = 180 etc.
                                s->depth = l->index + 100; // Todo: Deal with tilemap depth.
                        }
                }
        }
}