#include "dpl/utils.h"


const char *dpl_skip_whitespaces (const char *s)
{
    const char *end;
    if (s) {

        while (*s && (*s == ' ' || *s == '\t' || *s == '\n')) {
            s++;
        }

        end = s + strlen (s) - 1;
        while (   end >= s 
               && *end 
               && (*end == ' ' || *end == '\t' || *end == '\n')) {
            *(char *)end = '\0';
            end -= 1;
        }

        return s;
    }

    return 0;
}
