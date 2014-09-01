#pragma once

#include <stdbool.h>

typedef enum {
        log_err,
        log_warn,
        log_info,
        log_dbg
} log_level;

// Creates the file to which log messages will be written.
bool log_init(const char* log_file_path, log_level level);
// Logs the specified message.
void log_msg(log_level level, const char* level_name, 
                              const char *fmt, ...);
// Closes the log file if it is open.
void log_free();

#define LOGDBG(format, ...) log_msg(log_dbg, "DBG", format, __VA_ARGS__)
#define LOGINFO(format, ...) log_msg(log_info, "INFO", format, __VA_ARGS__)
#define LOGWARN(format, ...) log_msg(log_warn, "WARN", format, __VA_ARGS__)
#define LOGERR(format, ...) log_msg(log_err, "ERR", format, __VA_ARGS__)
