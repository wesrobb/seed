//#include "tilemap.h"
//
//#include <assert.h>
//#include <stdio.h>
//
//#include <log/log.h>
//
//#define MAX_TILE_MAPS 5
//
//bool parse_map_file(tilemap* tm, const char* map_file);
//void update_spritebatch(tilemap* tm);
//
//bool tilemap_init(tilemap* tm, const char* map_file,
//                  const char* atlas_file, const char* sprite_sheet_file)
//{
//        assert(tm);
//
//        if (!parse_map_file(tm, map_file)) {
//                return false;
//        }
//
//        if (!spritebatch_init(&tm->spritebatch,
//                              atlas_file, sprite_sheet_file)) {
//                return false;
//        }
//
//        update_spritebatch(tm);
//
//        return true;
//}
//
//void tilemap_draw(tilemap* tm)
//{
//        assert(tm);
//
//        spritebatch_draw(&tm->spritebatch);
//}
//
//bool parse_map_file(tilemap* tm, const char* map_file)
//{
//        FILE* map = fopen(map_file, "r");
//        if (!map) {
//                LOGERR("Failed to open map file %s", map_file);
//                return false;
//        }
//
//        // Read header
//        fscanf(map, "tileswide %d\n", &tm->tiles_wide);
//        fscanf(map, "tileshigh %d\n", &tm->tiles_high);
//        fscanf(map, "tilewidth %d\n", &tm->tile_width);
//        fscanf(map, "tileheight %d\n", &tm->tile_height);
//
//        tm->layer_count = 0;
//        while (!feof(map)) {
//                int layer = -1;
//                fscanf(map, "\nlayer %d\n", &layer);
//
//                // No more layers.
//                if (layer == -1) {
//                        break;
//                }
//
//                tm->layer_count++;
//                for (int i = tm->tiles_high - 1; i >= 0; --i) {
//                        for (int j = 0; j < tm->tiles_wide; ++j) {
//                                fscanf(map, "%hd,",
//                                       &tm->layers[layer].tile[i][j]);
//                        }
//                }
//        }
//
//        return true;
//}
//
//void update_spritebatch(tilemap* tm)
//{
//        for (int h = tm->layer_count - 1; h >= 0; --h) {
//                for (int i = 0; i < tm->tiles_high; ++i) {
//                        for (int j = 0; j < tm->tiles_wide; ++j) {
//                                short tile = tm->layers[h].tile[i][j];
//                                if (tile == -1) {
//                                        continue;
//                                }
//                                spritebatch_add_sprite_id(&tm->spritebatch,
//                                                          (int)tile,
//                                                          (float)j * tm->tile_width,
//                                                          (float)i * tm->tile_height,
//                                                          0, 0,
//                                                          (float)tm->tile_width,
//                                                          (float)tm->tile_height,
//                                                          0);
//                        }
//                }
//        }
//}