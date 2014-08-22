#include "mutex.h"

#include <assert.h>
#include <synchapi.h>

#include <log/log.h>

#include "types.h"
#include "win_error.h"

mutex* mutex_alloc()
{
        mutex* m = malloc(sizeof(m));
        if (!m) {
                LOGERR("%s", "Failed to allocate mutex");
                return NULL;
        }

        InitializeCriticalSection(&m->critical_section);
        return m;
}

void mutex_free(mutex* m)
{
        assert(m);

        DeleteCriticalSection(&m->critical_section);
        free(m);
}

void mutex_lock(mutex* m)
{
        assert(m);
        EnterCriticalSection(&m->critical_section);
}

void mutex_unlock(mutex* m)
{
        assert(m);
        LeaveCriticalSection(&m->critical_section);
}