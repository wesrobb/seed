#include "assets.h"

#include <assert.h>
#include <inttypes.h>

#include "khash.h"
#include "log.h"
#include "render.h"
#include "stretchy_buffer.h"
#include "texture.h"

#define PATH_MAX_LEN 127
#define MAX_TEXTURES 32
#define MAX_SOUNDS 32

static const int8_t unused_index = -1;

typedef struct texture_asset {
        uint8_t ref_count;
        int8_t path_len;
        char path[PATH_MAX_LEN];
} texture_asset;

typedef struct assets {
        // Space for the path to each texture_asset and texture.
        texture_asset texture_assets[MAX_TEXTURES];
        texture textures[MAX_TEXTURES];
} assets;

static assets s_assets;

void assets_init()
{
        // Mark the resources as unused
        for (int8_t i = 0; i < MAX_TEXTURES; ++i) {
                texture_asset* ta = &s_assets.texture_assets[i];
                ta->ref_count = 0;
                ta->path_len = -1;
        }
}

void assets_reset(renderer* r)
{
        for (int8_t i = 0; i < MAX_TEXTURES; ++i) {
                texture_asset* ta = &s_assets.texture_assets[i];
                texture* t = &s_assets.textures[i];

                if (ta->ref_count > 0) {
                        ta->ref_count = 0;
                        ta->path_len = -1;
                        texture_reset(t, r);
                }
        }
}

texture* assets_get_texture(const char* texture_path)
{
        // Check if the texture is already loaded.
        size_t path_len = strlen(texture_path);
        int8_t unused_texture_index = -1;
        for (int8_t i = 0; i < MAX_TEXTURES; ++i) {
                texture_asset* ta = &s_assets.texture_assets[i];
                if (path_len == ta->path_len && strcmp(texture_path, ta->path) == 0) {
                        ta->ref_count++;

                        LOGDBG("Assets: Texture %s already loaded", texture_path);
                        return &s_assets.textures[i];
                }

                // Save the first unused texture resource we find in case we have to load
                // the texture.
                if (unused_texture_index == -1 && ta->ref_count == 0) {
                        unused_texture_index = i;
                }
        }

        // Load the texture cos we didn't find it in the array of texture resources.
        if (unused_texture_index == -1) {
                LOGWARN("Unable to load texture %s because no more slots available", texture_path);
                return NULL;
        }

        texture* t = &s_assets.textures[unused_texture_index];
        if (!texture_init(t, unused_texture_index, texture_path)) {
                return NULL;
        }

        texture_asset* ta = &s_assets.texture_assets[unused_texture_index];
        ta->path_len = path_len;
        ta->ref_count++;
        strcpy(ta->path, texture_path);

        LOGDBG("Assets: Texture %s loaded", texture_path);
        return t;
}

void assets_release_texture(texture* t, renderer* r)
{
        assert(t);

        texture_asset* ta = &s_assets.texture_assets[t->id];
        ta->ref_count--;

        if (ta->ref_count == 0) {
                ta->path_len = -1;
                texture_reset(t, r);
        }
}