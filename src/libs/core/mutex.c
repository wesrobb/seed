#include "mutex.h"

#include <assert.h>
#include <Windows.h>

#include <log/log.h>

#include "win_error.h"

typedef struct mutex {
        HANDLE handle;
} mutex;

mutex* mutex_alloc()
{
        mutex* m = malloc(sizeof(m));
        if (!m) {
                LOGERR("%s", "Failed to allocate mutex");
                return NULL;
        }

        m->handle = CreateMutex(NULL, FALSE, NULL);
        if (!m->handle) {
                LOGERR("Failed to create mutex: %s", win_error_string());
                free(m);
                return NULL;
        }

        return m;
}

void mutex_free(mutex* m)
{
        assert(m);

        ReleaseMutex(m->handle);
        free(m);
}

bool mutex_lock(mutex* m)
{
        assert(m);

        DWORD result = WaitForSingleObject(m->handle, INFINITE);
        switch (result) {
        // The thread got ownership of the mutex
        case WAIT_OBJECT_0:
                return true;

        // The thread got ownership of an abandoned mutex
        case WAIT_ABANDONED:
                LOGERR("%s", "Received ownership of abandoned mutex");
                return false;

        default:
                LOGERR("%s", "Unknown mutex wait result");
                return false;
        }

}

bool mutex_unlock(mutex* m)
{
        assert(m);

        if (!ReleaseMutex(m->handle)) {
                LOGERR("Failed to unlock mutex %s", win_error_string());
                return false;
        }

        return true;
}