#include "dpl_test.h"
#include "dplversion.h"


int test_unit_version (int argc, char *argv[])
{
    const char *version = 0;

    DPL_ASSERT_OK (dpl_version (&version));
    DPL_ASSERT_OK (version != 0);
    DPL_ASSERT_EQ (strcmp (version, DPL_VERSION), 0);

    return 0;
}
