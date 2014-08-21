#pragma once

#include <inttypes.h>
#include <stdbool.h>

typedef struct rect {
        int32_t x, y, w, h;
} rect;

static const rect rect_zero = {0, 0, 0, 0};

// Returns true if the specified rect is empty (that is 0,0,0,0).
bool rect_empty(rect*);