#include "dpl_test.h"


int test_unit_task_ref (int argc, char *argv[])
{
    DplEntry *t;
    DplEntry *r_in;
    const DplEntry *r_out;

    DPL_ASSERT_OK (dpl_entry_new (&t, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&r_in, ENTRY_TASK));
    DPL_ASSERT_OK (dpl_entry_work_task_set (t, r_in));
    DPL_ASSERT_OK (dpl_entry_work_task_get (t, &r_out));
    DPL_ASSERT_EQ (r_in, r_out);

    DPL_ASSERT_OK (dpl_entry_free (t));
    DPL_ASSERT_OK (dpl_entry_free (r_in));

    return 0;
}
