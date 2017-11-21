/* +
 */


#ifndef DPL_UTILS_H
# define DPL_UTILS_H

# include <string.h>
# include "dpl/defs.h"


#define DPL_STRCPY(DPL_dst, DPL_src, DPL_dstsize) { \
    int DPL_pos = strlen (DPL_src); \
    if (DPL_pos >= DPL_dstsize) { \
        DPL_dstsize *= 2; \
        if (!(DPL_dst = realloc (DPL_dst, DPL_dstsize))) { \
            free (DPL_dst); \
            return DPL_ERR_MEM; \
        } \
    } \
    strcpy (DPL_dst, DPL_src); \
}
/* Copy a string. Reallocate memory if needed.
 *
 * Preconditions
 *   - DPL_src is not 0.
 *   - DPL_dstsize is the size of the buffer DPL_dst.
 *
 * Preconditions
 *   - The size of DPL_src is less than DPL_dstsize.
 * Postconditions
 *   - The content of DPL_src is copied to DPL_dst.
 *
 * Preconditions
 *   - The size of DPL_src is more than or equal to DPL_dstsize.
 *   - Memory can be allocated.
 * Postconditions
 *   - DPL_dst is enlarged to twice the size of DPL_src.
 *   - The content of DPL_src is copied to DPL_dst.
 *
 * Preconditions
 *   - The size of DPL_src is more than or equal to DPL_dstsize.
 *   - Memory cannot be allocated.
 * Postconditions
 *   - DPL_ERR_MEM is returned from the calling functions.
 *   - DPL_dst is freed.
 */


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
 * Precondition
 *   - DPL_src is 0
 * Postcondition
 *   - If DPL_dst is not 0, it is freed and set to 0
 *
 * Precondition
 *   - DPL_src is not 0
 *   - memory can be allocated
 * Postcondition
 *   - If DPL_dst is not 0, it is freed
 *   - DPL_dst is allocated to the size of DPL_src
 *   - The contents of DPL_src are copied to DPL_dst
 *
 * Precondition
 *   - DPL_src is not 0
 *   - memory cannot be allocated
 * Postcondition
 *   - If DPL_dst is not 0, it is freed
 *   - A return with DPL_ERR_MEM is invoked
 */


#define DPL_FORWARD_ERROR(DPL_value) { \
    int DPL_r = DPL_value; \
    if (DPL_r != DPL_OK) { \
        return DPL_r; \
    } \
}
/* Forward error codes.
 *
 * Precondition
 *   - DPL_value resolves to DPL_OK
 * Postcondition
 *   - Program flow continues.
 *
 * Precondition
 *   - DPL_value does not resolve to DPL_OK
 * Postcondition
 *   - return is invoked with the value DPL_value resolves to
 */


char *dpl_skip_whitespaces (char *s);
/* Strip trailing and leading whitespace characters from a string.
 *
 * Precondition
 *   - s is 0
 * Postcondition
 *   - 0 is returned
 *
 * Precondition
 *   - s is an allocated string
 * Postcondition
 *   - the return value points to the first position in s that does not contain
 *   a whitespace character (a line break, blank or tab)
 *   - s is truncated to end at the last character of s that is not a
 *   whitespace character
 */


#endif /* DPL_UTILS_H */
