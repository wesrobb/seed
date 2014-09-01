#pragma once

#include <stdint.h>

#include <synchapi.h>

typedef struct thread {
        uint32_t id;
        HANDLE handle;
} thread;

typedef struct mutex {
        CRITICAL_SECTION critical_section;
} mutex;

typedef struct condition_var {
        CONDITION_VARIABLE condition_variable;
} condition_var;