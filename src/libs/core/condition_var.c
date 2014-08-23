#include "condition_var.h"

#include <assert.h>
#include <stdlib.h>

#include <Windows.h>

#include <log/log.h>

#include "types.h"

condition_var* condition_var_create()
{
        condition_var* c = malloc(sizeof(*c));
        if (!c) {
                return NULL;
        }

        InitializeConditionVariable(&c->condition_variable);
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

        SleepConditionVariableCS(&c->condition_variable, 
                                 &m->critical_section, INFINITE);
}

void condition_var_notify(condition_var* c)
{
        assert(c);
        WakeConditionVariable(&c->condition_variable);
}

void condition_var_notify_all(condition_var* c)
{
        assert(c);
        WakeAllConditionVariable(&c->condition_variable);
}