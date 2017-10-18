#include "dpl_test.h"


int test_unit_parse_task_list_ref_conflict_1 (int argc, char *argv[])
{
    DplList *tasks;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1  Task 1\n"
                "2017-09-12\n"
                "  #1  Task 2\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));

    DPL_ASSERT_EQ (dpl_parse (DPL_tmpfile, &tasks, 1), DPL_ERR_SYNTAX);

    remove (DPL_tmpfile);

    return 0;
}
