#include "texture.h"

#include <assert.h>

#include <log/log.h>

#include "render.h"

unsigned char* stbi_load(const char*, int*, int*, int*, int);
void stbi_image_free(void *);

bool Texture_init(Texture* t, const char* location)
{
        unsigned char* data = stbi_load(location, 
                                        &t->width, &t->height, 
                                        &t->channels, 4);
        if (!data) {
                LOGERR("Failed to load texture %s", location);
                return false;
        }

        t->id = Render_loadTexture(data, t->width, t->height, t->channels);

        stbi_image_free(data);

        return true;
}
