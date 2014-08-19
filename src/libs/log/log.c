#include "log.h"

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static FILE* _log_file = NULL;
log_level _log_level;

bool log_init(const char* log_file_path, log_level level)
{
        _log_level = level;
        _log_file = fopen(log_file_path, "a");
        if (_log_file) {
                LOGDBG("Successfully opened log file: %s", log_file_path);
                return true;
        }
        else {
                LOGERR("Failed to open log file: %s", log_file_path);
                return false;
        }
}

void log_msg(log_level level, const char *level_name, const char *fmt, ...)
{
        if (level <= _log_level) {
                va_list args;
                va_start(args, fmt);

                time_t now;
                time(&now);
                char time_buffer[18];
                strftime(time_buffer, 18, "%y-%m-%d %H:%M:%S", gmtime(&now));

                // Log to console.
                static const char* prefix_fmt = "%s [%s]: ";
                printf(prefix_fmt, time_buffer, level_name);
                vprintf(fmt, args);
                fputs("\n", stdout);
                fflush(stdout);

                va_end(args);

                // Log to file.
                if (_log_file) {
                        va_list file_args;
                        va_start(file_args, fmt);

                        fprintf(_log_file, prefix_fmt, time_buffer, level_name);
                        vfprintf(_log_file, fmt, file_args);
                        fputs("\n", _log_file);

                        va_end(file_args);
                }
        }
}

void log_free()
{
        if (_log_file) {
                fclose(_log_file);
        }
}
