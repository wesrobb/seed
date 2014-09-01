#pragma once

#include <stdbool.h>

#include <core/rect.h>

// Todo: Convert floats to kmVec2 where appropriate.
typedef struct sprite {
        float x_pos;
        float y_pos;
        float x_anchor;
        float y_anchor;
        float scale;
        float rotation;
        int8_t depth;
        bool flip_x;
        rect tex_rect; // Region of the texture to be drawn for this sprite.
        struct texture* tex;
} sprite;

