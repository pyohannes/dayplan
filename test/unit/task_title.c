#include "dpl_test.h"


int test_unit_task_title (int argc, char *argv[])
{
    const char *title_in = "Projects/Dpl/Coding";
    const char *title_out;
    DplTask *t;

    DPL_ASSERT_OK (dpl_task_new (&t));
    DPL_ASSERT_OK (dpl_task_title_set (t, "Projects/Dpl/Coding"));
    DPL_ASSERT_OK (dpl_task_title_get (t, &title_out));
    DPL_ASSERT_EQ (strcmp (title_in, title_out), 0);

    DPL_ASSERT_OK (dpl_task_free (t));

    return 0;
}
