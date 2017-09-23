#include "dpl_test.h"


int test_task_begin (int argc, char *argv[])
{
    time_t begin_in = time (0);
    time_t begin_out = 0;
    DplTask *t;

    DPL_ASSERT_OK (dpl_task_new (&t));
    DPL_ASSERT_OK (dpl_task_begin_set (t, begin_in));
    DPL_ASSERT_OK (dpl_task_begin_get (t, &begin_out));
    DPL_ASSERT_EQ (begin_in, begin_out);

    DPL_ASSERT_OK (dpl_task_free (t));

    return 0;
}
