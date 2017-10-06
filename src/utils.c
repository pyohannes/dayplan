#include "dpl/utils.h"


char *dpl_skip_whitespaces (char *s)
{
#define IS_WHITE(DPL_s) \
    (DPL_s && (DPL_s == ' ' || DPL_s == '\t' || DPL_s == '\n'))

    char *end;

    if (s) {
        while (IS_WHITE(*s)) {
            s++;
        }

        end = s + strlen (s) - 1;
        while (end >= s && IS_WHITE(*end)) {
            *end = '\0';
            end--;
        }

        return s;
    }

    return 0;
}
