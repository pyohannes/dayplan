#include "dpl_test.h"


int test_unit_list_new (int argc, char *argv[])
{
    DplList *tasks;
    uint32_t len;

    DPL_ASSERT_OK (dpl_list_new (&tasks));
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 0);

    DPL_ASSERT_OK (dpl_list_free (tasks, 0));

    return 0;
}
