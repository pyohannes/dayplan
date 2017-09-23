#include "dpl_test.h"


int test_task_ref (int argc, char *argv[])
{
    DplTask *t;
    DplRef *r_in;
    const DplRef *r_out;

    DPL_ASSERT_OK (dpl_task_new (&t));
    DPL_ASSERT_OK (dpl_ref_new (&r_in));
    DPL_ASSERT_OK (dpl_task_ref_set (t, r_in));
    DPL_ASSERT_OK (dpl_task_ref_get (t, &r_out));
    DPL_ASSERT_EQ (r_in, r_out);

    /* empty task owns ref */
    DPL_ASSERT_OK (dpl_task_free (t));

    return 0;
}
