#pragma once

#include <stdbool.h>

typedef struct rect {
        float x, y, w, h;
} rect;

static const rect rect_zero = {0.0f, 0.0f, 0.0f, 0.0f};

// Returns true if the specified rect is empty (that is 0.0f,0.0f,0.0f,0.0f).
bool rect_empty(rect*);