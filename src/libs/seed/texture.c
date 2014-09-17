#include "texture.h"

#include <assert.h>
#include <stddef.h>

#include "log.h"
#include "render.h"

unsigned char* stbi_load(const char*, int*, int*, int*, int);
void stbi_image_free(void *);

bool texture_init(texture* t, int8_t id, const char* location)
{
        assert(t);

        t->data = stbi_load(location,
                            &t->width, &t->height,
                            &t->channels, 4);
        if (!t->data) {
                LOGERR("Failed to load texture %s", location);
                return false;
        }

        t->id = id;
        t->uploaded = false;

        return true;
}

void texture_reset(texture* t, renderer* r)
{
        assert(t);

        render_delete_texture(r, t);

        t->id = -1;
        t->gl_id = -1;

        t->width = 0;
        t->height = 0;
        t->channels = 0;

        stbi_image_free(t->data);
        t->data = NULL;
}