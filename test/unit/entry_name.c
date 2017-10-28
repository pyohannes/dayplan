#include "dpl_test.h"


int test_unit_entry_name (int argc, char *argv[])
{
    const char *name_in = "Projects/Dpl/Coding";
    const char *name_out;
    DplEntry *t;

    DPL_ASSERT_OK (dpl_entry_new (&t, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_name_set (t, "Projects/Dpl/Coding"));
    DPL_ASSERT_OK (dpl_entry_name_get (t, &name_out));
    DPL_ASSERT_EQ (strcmp (name_in, name_out), 0);

    DPL_ASSERT_OK (dpl_entry_free (t));

    return 0;
}
