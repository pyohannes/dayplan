#include "dpl_test.h"


int test_unit_parse_task_list_3 (int argc, char *argv[])
{
    DplEntry *task;
    DplList *tasks;
    DplIter *iter, *done_iter;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1  Task 1\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 1);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_filter_done (iter, 1, &done_iter));

    DPL_ASSERT_EQ (dpl_iter_next (done_iter, &task), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_iter_free (done_iter));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
