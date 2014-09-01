#include "thread.h"

#include <assert.h>
#include <stddef.h>

#include "../log.h"

#include <process.h>
#include <Windows.h>

#include "types.h"
#include "win_error.h"

void set_thread_name(uint32_t thread_id, const char* name);

thread* thread_create(const char* thread_name,
                      thread_fn fn, void* fn_arg)
{
        thread* t = malloc(sizeof(*t));
        if (!t) {
                LOGERR("Failed to allocate thread %s", thread_name);
                return NULL;
        }

        t->handle = (HANDLE)_beginthreadex(NULL, 0, fn, fn_arg, 0, &t->id);
        if (!t->handle) {
                LOGERR("Failed to create thread handle for thread %s: %s",
                       thread_name, win_error_string());
                free(t);
                return NULL;
        }

        set_thread_name(t->id, thread_name);

        return t;
}

void thread_join(thread* t)
{
        assert(t);
        WaitForSingleObject(t->handle, INFINITE);
}

void thread_free(thread* t)
{
        assert(t);
        CloseHandle(t->handle);
        free(t);
}

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct thread_info {
        DWORD type; // Must be 0x1000.
        LPCSTR name; // Pointer to name (in user addr space).
        DWORD thread_id; // Thread ID (-1=caller thread).
        DWORD flags; // Reserved for future use, must be zero.
} thread_info;
#pragma pack(pop)

void set_thread_name(uint32_t thread_id, const char* name)
{
        thread_info info;
        info.type = 0x1000;
        info.name = name;
        info.thread_id = thread_id;
        info.flags = 0;

        __try {
                RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {}
}