#include "rect.h"

#include <assert.h>

bool rect_empty(rect* r)
{
        assert(r);
        return r->x == 0 && r->y == 0 && r->w == 0 && r->h;
}