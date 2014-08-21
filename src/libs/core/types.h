#pragma once

#include <stdint.h>

#include <Windows.h>

typedef struct thread {
        uint32_t id;
        HANDLE handle;
} thread;

typedef struct mutex {
        HANDLE handle;
} mutex;

typedef struct condition_var {
        CONDITION_VARIABLE handle;
} condition_var;