#pragma once

#include <inttypes.h>

static const float g_default_tex_coords[] = { 0.0f, 1.0f,   // bottom left
                                              0.0f, 0.0f,   // top left
                                              1.0f, 0.0f,   // top right
                                              1.0f, 1.0f }; // bottom right


typedef struct sprite {
        float x_pos;
        float y_pos;
        float width;
        float height;
        float x_anchor;
        float y_anchor;
        float rotation;
        float* tex_coords; // Order is bl, tl, tr, br.
        struct sprite *prev, *next;
        uint32_t id;
        struct texture* texture;
} sprite;
