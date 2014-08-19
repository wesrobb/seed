#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct Texture
{
        uint32_t id;

        int width;
        int height;
        int channels;
} Texture;

/// Initializes the specified texture from the specified file and returns in.
/// Returns false if initialization failed. Errors will be logged.
bool Texture_init(Texture*, const char* location);
