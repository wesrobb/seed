#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct texture
{
        uint32_t id;

        int width;
        int height;
        int channels;
} texture;

/// Initializes the specified texture from the specified file and returns in.
/// Returns false if initialization failed. Errors will be logged.
bool texture_init(texture*, const char* location);
