#include "dpl/utils.h"


const char *dpl_skip_whitespaces (const char *s)
{
    if (s) {
        while (*s && *s == ' ' || *s == '\t') {
            s++;
        }
        return s;
    }

    return 0;
}
