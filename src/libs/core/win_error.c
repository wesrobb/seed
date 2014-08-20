#include "win_error.h"

#include <Windows.h>

#define MAX_MSG_LEN 512

static char s_error_string[MAX_MSG_LEN];

const char* win_error_string()
{
        DWORD error = GetLastError();
        if (!error) {
                return NULL;
        }

        char* msg_buffer = NULL;
        DWORD buffer_len = FormatMessageA(
                                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                FORMAT_MESSAGE_FROM_SYSTEM |
                                FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL,
                                error,
                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                msg_buffer,
                                0, NULL);
        if (buffer_len) {
                DWORD len = min(buffer_len, MAX_MSG_LEN);
                char* result = strcpy(s_error_string, msg_buffer);
                LocalFree(msg_buffer);

                return result;
        }

        return NULL;
}