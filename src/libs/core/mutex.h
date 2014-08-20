#pragma once

#include <stdbool.h>

typedef struct mutex mutex;

// Allocates a new mutex.
mutex* mutex_alloc();
// Frees the specified mutex.
void mutex_free(mutex* m);

// Locks the specified mutex.
// Returns false if locking failed.
bool mutex_lock(mutex* m);
// Unlocks the specified mutex.
// Returns false if unlocking failed.
bool mutex_unlock(mutex* m);

