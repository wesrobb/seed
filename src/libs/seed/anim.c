#include "anim.h"

#include <spine/extension.h>

#include "assets.h"
#include "log.h"
#include "texture.h"

/// Required by spine.
void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
        texture* t = assets_get_texture(path);
        if (!t) {
                LOGERR("Failed to load texture %s for spine animation", path);
                return;
        }
        self->rendererObject = t;
        self->width = t->width;
        self->height = t->height;
}

/// Required by spine.
void _spAtlasPage_disposeTexture(spAtlasPage* self)
{

}