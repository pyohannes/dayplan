/* +
 */

#include <stdlib.h>
#include "dpl_test.h"


int dpl_test_write (char *outfile, size_t outfilelen, const char *content)
{
    const char *template = "dplXXXXXX";
    int fdesc;
    int contentlen = strlen (content);

    strncpy (outfile, template, outfilelen);

    if ((fdesc = mkstemp (outfile)) == -1) {
        return DPL_ERR_IO;
    }

    if (write (fdesc, content, contentlen) != contentlen) {
        return DPL_ERR_IO;
    }

    if (close (fdesc) == -1) {
        return DPL_ERR_IO;
    }

    return DPL_OK;
}
