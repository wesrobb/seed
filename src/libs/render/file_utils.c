#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>

#include <log/log.h>

void* file_contents(const char* filename, int32_t* length)
{
        FILE *f = fopen(filename, "r");
        void *buffer;

        if (!f) {
                LOGERR("Unable to open %s for reading", filename);
                return NULL;
        }

        fseek(f, 0, SEEK_END);
        *length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = malloc(*length+1);
        *length = fread(buffer, 1, *length, f);
        fclose(f);
        ((char*)buffer)[*length] = '\0';

        return buffer;
}
