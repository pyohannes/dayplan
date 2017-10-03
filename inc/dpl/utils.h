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


#define DPL_FORWARD_ERROR(DPL_value) { \
    int DPL_r = DPL_value; \
    if (DPL_r != DPL_OK) { \
        return DPL_r; \
    } \
}


const char *dpl_skip_whitespaces (const char *s);


#endif /* DPL_UTILS_H */
