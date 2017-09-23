#include "dpl_test.h"


int test_task_end (int argc, char *argv[])
{
    time_t end_in = time (0);
    time_t end_out = 0;
    DplTask *t;

    DPL_ASSERT_OK (dpl_task_new (&t));
    DPL_ASSERT_OK (dpl_task_end_set (t, end_in));
    DPL_ASSERT_OK (dpl_task_end_get (t, &end_out));
    DPL_ASSERT_EQ (end_in, end_out);

    DPL_ASSERT_OK (dpl_task_free (t));

    return 0;
}
