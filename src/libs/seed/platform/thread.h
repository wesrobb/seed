#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct thread thread;
typedef uint32_t(__stdcall *thread_fn)(void*);

// Creates a new thread to execute the specified function.
// Returns NULL if creation failed.
thread* thread_create(const char* thread_name, thread_fn fn, void* fn_arg);

// Blocks until the specified thread is finished processing.
void thread_join(thread* t);

// Terminates the thread if its running and frees it's memory.
void thread_free(thread* t);