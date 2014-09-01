#include "atlas.h"

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include <core/rect.h>
#include <core/stretchy_buffer.h>
#include <log/log.h>

#include "sprite.h"
#include "texture.h"

unsigned char* stbi_load(const char*, int*, int*, int*, int);
void stbi_image_free(void *);
bool parse_atlas_info(atlas*, const char* atlas_info_path);

bool atlas_init(atlas* a,
                texture* texture,
                const char* atlas_info_path)
{
        assert(a);
        assert(texture);

        a->texture = texture;
        a->rect_sb = NULL;
        a->sprite_name_sb = NULL;
        if (!parse_atlas_info(a, atlas_info_path)) {
                return false;
        }

        return true;
}

void atlas_reset(atlas* a)
{
        assert(a);

        sb_free(a->rect_sb);
        sb_free(a->sprite_name_sb);
        kh_destroy(sprite_map, a->sprite_names_map);
}

bool atlas_sprite_name(atlas* a, sprite* s,
                       const char* name,
                       float x_pos, float y_pos,
                       float x_anchor, float y_anchor,
                       float scale,
                       float rotation)
{
        // Find the ID in the hash map using the name.
        khiter_t iter = kh_get(sprite_map, a->sprite_names_map, name);
        if (iter == kh_end(a->sprite_names_map)) {
                LOGERR("Failed to find sprite %s when adding to atlas", name);
                return false;
        }

        int32_t sprite_id = kh_val(a->sprite_names_map, iter);
        return atlas_sprite_id(a, s,
                               sprite_id,
                               x_pos, y_pos,
                               x_anchor, y_anchor,
                               scale,
                               rotation);
}

bool atlas_sprite_id(atlas* a, sprite* s,
                     int32_t sprite_id,
                     float x_pos, float y_pos,
                     float x_anchor, float y_anchor,
                     float scale,
                     float rotation)
{
        if (sprite_id < 0 || sprite_id >= sb_count(a->rect_sb)) {
                return false;
        }

        s->x_pos = x_pos;
        s->y_pos = y_pos;
        s->x_anchor = x_anchor;
        s->y_anchor = y_anchor;
        s->scale = scale;
        s->rotation = rotation;
        s->tex_rect = a->rect_sb[sprite_id];
        s->tex = a->texture;

        return true;
}

bool parse_atlas_info(atlas *a, const char* atlas_info_path)
{
        FILE* atlas_file = fopen(atlas_info_path, "r");
        if (!atlas_file) {
                LOGERR("Failed to open atlas file %s for reading",
                       atlas_info_path);
                return false;
        }

        int num_images = 0;
        int ch;
        while (EOF != (ch = fgetc(atlas_file))) {
                if (ch == '\n') {
                        ++num_images;
                }
        }
        rewind(atlas_file);

        a->sprite_names_map = kh_init(sprite_map);
        struct sprite_name* name = sb_add(a->sprite_name_sb, num_images);
        for (int i = 0; i < num_images; ++i) {
                rect* r = sb_add(a->rect_sb, 1);

                static const char* format = "%[^:]:%f:%f:%f:%f%*[\n]";

                // Parse data out the file.
                fscanf(atlas_file, format,
                       name[i].name,
                       &r->x, &r->y, &r->w, &r->h);

                // Add the sprite name.
                int kh_ret;
                khiter_t iter = kh_put(sprite_map,
                                       a->sprite_names_map,
                                       name[i].name,
                                       &kh_ret);
                kh_val(a->sprite_names_map, iter) = i;
        }

        fclose(atlas_file);
        return true;
}

