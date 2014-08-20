#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct texture {
        uint32_t id;

        int width;
        int height;
        int channels;

        unsigned char* data;
        struct mutex* data_mutex;

        bool uploaded;
} texture;

// Initializes the specified texture from the specified file.
// Returns false if initialization failed. Errors will be logged.
bool texture_init(texture*, const char* location);

// Uploads the specified texture to the for rendering. This will lock the
// texture while the data is copied to the GL context.
bool texture_upload(texture*);

// Deletes the texture data and resets all fields to 0.
// Also removes the texture data from GPU memory if it was uploaded.
void texture_free(texture* t);
