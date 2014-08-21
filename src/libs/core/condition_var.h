#pragma once

typedef struct condition_var condition_var;

// Creats a new condition variable.
// Returns NULL if creation fails.
condition_var* condition_var_create();

// Frees the specified condition variable.
void condition_var_free(condition_var*);

// Blocks the current thread until the condition variable is woken up.
// Note that the mutex must be locked before calling this function.
void condition_var_wait(condition_var*, struct mutex* m);

// Notifies one waiting thread.
void condition_var_notify(condition_var*);

// Notifies all waiting threads.
void condition_var_notify_all(condition_var*);