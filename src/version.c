/* +
 */


#include "dplversion.h"
#include "dpl/version.h"
#include "dpl/defs.h"


int dpl_version (const char **version)
{
    *version = DPL_VERSION;

    return DPL_OK;
}
