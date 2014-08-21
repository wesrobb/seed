#include "condition_var.h"

#include <assert.h>
#include <stdlib.h>

#include <log/log.h>

#include "types.h"

condition_var* condition_var_create()
{
        condition_var* c = malloc(sizeof(*c));
        if (!c) {
                LOGERR("%s", "Failed to allocate condition variable");
                return NULL;
        }

        return c;
}

void condition_var_free(condition_var* c)
{
        assert(c);
        free(c);
}

void condition_var_wait(condition_var* c, struct mutex* m)
{
        assert(c);
        assert(m);

        SleepConditionVariableCS(&c->handle, m->handle, INFINITE);
}

void condition_var_notify(condition_var* c)
{
        assert(c);
        WakeConditionVariable(&c->handle);
}

void condition_var_notify_all(condition_var* c)
{
        assert(c);
        WakeAllConditionVariable(&c->handle);
}