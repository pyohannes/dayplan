/* +
 */


#ifndef DPL_UTILS_H
# define DPL_UTILS_H

# include <string.h>
# include "dpl/defs.h"


#define DPL_STRDUP(DPL_dst, DPL_src) \
    if (DPL_dst) { \
        free (DPL_dst); \
    } \
    if (!(DPL_src)) { \
        DPL_dst = 0; \
    } else { \
        DPL_dst = malloc (strlen (DPL_src) + 1); \
        if (!DPL_dst) { \
            return DPL_ERR_MEM; \
        } \
        strcpy (DPL_dst, DPL_src); \
    }
/* Duplicate a string.
 *
 * DPL_dst is freed if it is not null. If DPL_src is null, DPL_dst is set to
 * null.
 *
 * If DPL_src is not null, DPL_dst is allocated to the size of DPL_dst and the 
 * contents of DPL_src are copied to DPL_dest.
 *
 * If DPL_dst cannot be allocated, DPL_ERR_MEM is returned from the function
 * invoking the macro.
 */


#define DPL_FORWARD_ERROR(DPL_value) { \
    int DPL_r = DPL_value; \
    if (DPL_r != DPL_OK) { \
        return DPL_r; \
    } \
}
/* Forward error codes.
 *
 * If DPL_value evaluates to a value different than DPL_OK, the function
 * invoking the macro returns this value.
 */


char *dpl_skip_whitespaces (char *s);
/* Strip trailing and leading whitespace characters from a string.
 *
 * The return value is a substring of s. It begins at the first
 * character of s that is not a whitespace character, it ends at
 * the last character of s that is not a whitespace character.
 *
 * The return value points into s. s can be altered during
 * this call.
 *
 * If s is 0, 0 is returned.
 *
 * Whitespace characters are line breaks, blanks and tabs.
 */


#endif /* DPL_UTILS_H */
