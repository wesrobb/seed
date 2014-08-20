#include "spritebatch.h"

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#include <log/log.h>

#include "render.h"

/// Buffers passed to GL. Reused by each spritebatch.
static float _vert_data[SPRITEBATCH_MAX_SPRITES * 6 * 2];
static float _tex_data[SPRITEBATCH_MAX_SPRITES * 6 * 2];

unsigned char* stbi_load(const char*, int*, int*, int*, int);
void stbi_image_free(void *);
bool parse_atlas_info(spritebatch*, const char* atlas_info_path);

bool spritebatch_init(spritebatch *sb,
                      const char *atlas_info_path,
                      const char *texture_atlas_path)
{
        assert(sb);
        assert(atlas_info_path);
        assert(texture_atlas_path);

        memset(sb, 0, sizeof(sb));
        if (!parse_atlas_info(sb, atlas_info_path)) {
                return false;
        }

        if (!texture_init(&sb->texture, texture_atlas_path)) {
                LOGERR("Failed to create texture %s", texture_atlas_path);
                return false;
        }

        if (!texture_upload(&sb->texture)) {
                LOGERR("Failed to upload texture %s", texture_atlas_path);
                return false;
        }

        spritebatch_clear_sprites(sb);

        return true;
}

void spritebatch_destroy(spritebatch* sb)
{
        assert(sb);

        texture_free(&sb->texture);
        kh_destroy(sp, sb->sprite_names_map);
}

void spritebatch_draw(const spritebatch* sb)
{
        assert(sb);
        for (sprite* s = sb->active_sprites; s; s = s->next) {
                render_add_sprite(s);
        }
}

sprite* spritebatch_add_sprite_name(spritebatch* sb,
                                    const char* name,
                                    float x_pos, float y_pos,
                                    float x_anchor, float y_anchor,
                                    float width, float height,
                                    float rotation)
{
        // Find the ID in the hash map using the name.
        khiter_t iter = kh_get(sp, sb->sprite_names_map, name);
        if (iter == kh_end(sb->sprite_names_map)) {
                return NULL;
        }

        uint32_t sprite_id = kh_val(sb->sprite_names_map, iter);
        return spritebatch_add_sprite_id(sb,
                                         sprite_id,
                                         x_pos, y_pos,
                                         x_anchor, y_anchor,
                                         width, height,
                                         rotation);
}

sprite* spritebatch_add_sprite_id(spritebatch* sb,
                                  uint32_t sprite_id,
                                  float x_pos, float y_pos,
                                  float x_anchor, float y_anchor,
                                  float width, float height,
                                  float rotation)
{
        if (!sb->inactive_sprites) {
                // No more sprites available.
                return NULL;
        }

        sprite* s;
        s = sb->inactive_sprites;
        sb->inactive_sprites = sb->inactive_sprites->next;

        // Inactive sprite may be empty now
        if (sb->inactive_sprites) {
                sb->inactive_sprites->prev = NULL;
        }

        s->prev = NULL;
        s->next = sb->active_sprites;
        if (sb->active_sprites) {
                // Only set the previous node if there is one to set.
                sb->active_sprites->prev = s;
        }
        sb->active_sprites = s;

        s->id = sprite_id;
        s->x_pos = x_pos;
        s->y_pos = y_pos;
        s->x_anchor = x_anchor;
        s->y_anchor = y_anchor;
        s->width = width;
        s->height = height;
        s->rotation = rotation;
        s->texture = &sb->texture;
        s->tex_coords = &sb->tex_coords[s->id * 8];

        return s;
}

void spritebatch_remove_sprite(spritebatch* sb, sprite** s)
{
        assert(sb);
        assert(s);
        assert(*s);

        // Remove from the active list
        if ((*s)->next == NULL && (*s)->prev == NULL) {
                sb->active_sprites = NULL;
        } else if ((*s)->next == NULL) {
                // End of active list.
                (*s)->prev->next = NULL;
        } else if ((*s)->prev == NULL) {
                // Head of the list.
                sb->active_sprites = (*s)->next;
                sb->active_sprites->prev = NULL;
        }

        (*s)->prev = NULL;
        (*s)->next = sb->inactive_sprites;
        sb->inactive_sprites->prev = *s;
        sb->inactive_sprites = *s;
        *s = NULL;
}

void spritebatch_clear_sprites(spritebatch* sb)
{
        memset(sb->sprites, 0, sizeof(sb->sprites));
        for (uint32_t i = 0; i < SPRITEBATCH_MAX_SPRITES; ++i) {
                if (i == 0) {
                        sb->sprites[i].prev = NULL;
                        sb->sprites[i].next = &sb->sprites[i + 1];
                        continue;
                } else if (i == SPRITEBATCH_MAX_SPRITES - 1) {
                        sb->sprites[i].prev = &sb->sprites[i - 1];
                        sb->sprites[i].next = NULL;
                        break;
                }
                sb->sprites[i].prev = &sb->sprites[i - 1];
                sb->sprites[i].next = &sb->sprites[i + 1];
        }

        sb->active_sprites = NULL;
        sb->inactive_sprites = &sb->sprites[0];
}

bool parse_atlas_info(spritebatch *sb, const char* atlas_info_path)
{
        FILE* atlas_file = fopen(atlas_info_path, "r");
        if (!atlas_file) {
                LOGERR("Failed to open atlas file %s for reading",
                       atlas_info_path);
                return false;
        }

        // Skip the first line which contains width and height.
        sb->num_images = -1;
        int ch;
        while (EOF != (ch = fgetc(atlas_file))) {
                if (ch == '\n') {
                        ++sb->num_images;
                }
        }
        rewind(atlas_file);

        uint32_t image_width = 0;
        uint32_t image_height = 0;
        fscanf(atlas_file, "%d:%d%*[^\n][\n]", &image_width, &image_height);

        if (image_width == 0 || image_height == 0) {
                fclose(atlas_file);
                LOGERR("Atlas info file %s does not contain image's size",
                       atlas_info_path);
                return false;
        }

        memset(sb->tex_coords, 0, sizeof(sb->tex_coords));
        memset(sb->sprite_names, 0, sizeof(sb->sprite_names));
        sb->sprite_names_map = kh_init(sp);
        uint32_t tex_index = 0;
        for (uint32_t i = 0; i < sb->num_images; ++i) {
                float x = 0;
                float y = 0;
                float w = 0;
                float h = 0;

                static const char* format =
                        "%*[\n]%[^:]:\
                         %f:\
                         %f:\
                         %f:\
                         %f%*[^\n]";

                uint32_t name_pos = i * SPRITEBATCH_MAX_SPRITE_NAME_LEN;
                fscanf(atlas_file, format,
                       &sb->sprite_names[name_pos],
                       &x, &y, &w, &h);
                int kh_ret;
                khiter_t iter = kh_put(sp,
                                       sb->sprite_names_map,
                                       &sb->sprite_names[name_pos],
                                       &kh_ret);
                kh_value(sb->sprite_names_map, iter) = i;

                // Bottom left
                sb->tex_coords[tex_index++] = x / image_width;
                sb->tex_coords[tex_index++] = (y + h) / image_height;

                // Top left
                sb->tex_coords[tex_index++] = x / image_width;
                sb->tex_coords[tex_index++] = y / image_height;

                // Top right
                sb->tex_coords[tex_index++] = (x + w) / image_width;
                sb->tex_coords[tex_index++] = y / image_height;

                // Bottom right
                sb->tex_coords[tex_index++] = (x + w) / image_width;
                sb->tex_coords[tex_index++] = (y + h) / image_height;
        }

        fclose(atlas_file);
        return true;
}

