/* +
 */

#ifndef DPL_TEST_H
# define DPL_TEST_H


# include <string.h>
# include <stdio.h>
# include "dpl/dpl.h"


#define DPL_TMPFILE_LEN 1024
static char DPL_tmpfile[DPL_TMPFILE_LEN];


# define DPL_ASSERT_EQ(DPL_left, DPL_right) { \
    if ((DPL_left) != (DPL_right)) { \
        fprintf (stderr, "TEST ERROR %s:%d: Expected %ld, got %ld (%s)\n", \
                 __FILE__, __LINE__, (long) DPL_right, (long) DPL_left, \
                 #DPL_left); \
        return 1; \
    } \
}


# define DPL_ASSERT_NEQ(DPL_left, DPL_right) { \
    if ((DPL_left) == (DPL_right)) { \
        fprintf (stderr, "TEST ERROR %s:%d: Expected not %ld, got %ld\n", \
                 __FILE__, __LINE__, (long) DPL_right, (long) DPL_left); \
        return 1; \
    } \
}


# define DPL_ASSERT_OK(DPL_left) \
    DPL_ASSERT_EQ ((DPL_left), DPL_OK)


int dpl_test_write (char *outfile, size_t outfilelen, const char *content);


#endif /* DPL_TEST_H */
