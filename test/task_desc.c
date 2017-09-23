#include "dpl_test.h"


int test_task_desc (int argc, char *argv[])
{
    const char *desc_in = "This is a multiline\ndescription.";
    const char *desc_out;
    DplTask *t;

    DPL_ASSERT_OK (dpl_task_new (&t));
    DPL_ASSERT_OK (dpl_task_desc_set (t, desc_in));
    DPL_ASSERT_OK (dpl_task_desc_get (t, &desc_out));
    DPL_ASSERT_EQ (strcmp (desc_in, desc_out), 0);

    DPL_ASSERT_OK (dpl_task_free (t));

    return 0;
}
