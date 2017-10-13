#include "dpl_test.h"


int test_unit_task_title (int argc, char *argv[])
{
    const char *title_in = "Projects/Dpl/Coding";
    const char *title_out;
    DplEntry *t;

    DPL_ASSERT_OK (dpl_entry_new (&t, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_name_set (t, "Projects/Dpl/Coding"));
    DPL_ASSERT_OK (dpl_entry_name_get (t, &title_out));
    DPL_ASSERT_EQ (strcmp (title_in, title_out), 0);

    DPL_ASSERT_OK (dpl_entry_free (t));

    return 0;
}
