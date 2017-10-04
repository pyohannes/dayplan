#include "dpl_test.h"


int test_unit_tasklist_new (int argc, char *argv[])
{
    DplTaskList *tasks;
    uint32_t len;

    DPL_ASSERT_OK (dpl_tasklist_new (&tasks));
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 0);

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 0));

    return 0;
}
