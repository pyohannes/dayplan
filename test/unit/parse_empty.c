#include "dpl_test.h"


int test_unit_parse_empty (int argc, char *argv[])
{
    DplList *tasks;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                ""));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 0);

    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
