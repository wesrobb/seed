#include "texture.h"

#include <assert.h>
#include <stddef.h>

#include <core/mutex.h>
#include <log/log.h>

#include "render.h"

unsigned char* stbi_load(const char*, int*, int*, int*, int);
void stbi_image_free(void *);

bool texture_init(texture* t, const char* location)
{
        assert(t);

        t->data = stbi_load(location,
                            &t->width, &t->height,
                            &t->channels, 4);
        if (!t->data) {
                LOGERR("Failed to load texture %s", location);
                return false;
        }

        t->data_mutex = mutex_alloc("gl_texture_upload_mutex");
        if (!t->data_mutex) {
                LOGERR("%s", "Failed to allocate texture mutex");
                stbi_image_free(t->data);
                t->data = NULL;
                return false;
        }

        t->uploaded = false;

        return true;
}

bool texture_upload(texture* t)
{
        assert(t);

        if (!mutex_lock(t->data_mutex)) {
                LOGERR("%s", "Failed to lock texture data for upload");
                return false;
        }

        if (!render_load_texture(t)) {
                LOGERR("%s", "Failed to upload texture to GL");
                mutex_unlock(t->data_mutex);
                return false;
        }

        mutex_unlock(t->data_mutex);
        return true;
}

void texture_free(texture* t)
{
        assert(t);

        render_delete_texture(t);

        t->id = -1;

        t->width = 0;
        t->height = 0;
        t->channels = 0;

        mutex_free(t->data_mutex);
        t->data_mutex = NULL;
        stbi_image_free(t->data);
        t->data = NULL;
}