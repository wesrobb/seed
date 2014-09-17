#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct texture {
        uint8_t id;
        uint32_t gl_id;

        int width;
        int height;
        int channels;
        unsigned char* data;

        bool uploaded;
} texture;

// Initializes the specified texture from the specified file
// and assigns it the specified id.
// Returns false if initialization failed. Errors will be logged.
bool texture_init(texture*, int8_t id, const char* location);

// Deletes the texture data and resets all fields to 0.
// Also removes the texture data from GPU memory if it was uploaded.
void texture_reset(texture*, struct renderer* r);