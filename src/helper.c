#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "helper.h"

void start_timer()
{
    clock_gettime(CLOCK_MONOTONIC, &start);
}

void get_timer()
{
    clock_gettime(CLOCK_MONOTONIC, &current);
    elapsed = (current.tv_sec - start.tv_sec);
    elapsed += (current.tv_nsec - start.tv_nsec) / 1000000000.0;
}

void debug(int rank, char *format, ...)
{
    va_list args; /* Variable argument list */

    va_start(args, format);

    printf("%ld|%2d|", time(NULL), rank);
    vprintf(format, args);

    /* Clean up variable argument list */
    va_end(args);
}