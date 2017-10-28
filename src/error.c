#include <stdio.h>
#include <stdarg.h>
#include "dpl/error.h"


int dpl_error (const char *fmt, ...)
{
    va_list args;

    va_start (args, fmt);

    return vfprintf (stderr, fmt, args);
}
