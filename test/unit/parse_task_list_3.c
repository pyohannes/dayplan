#include "dpl_test.h"


int test_unit_parse_task_list_3 (int argc, char *argv[])
{
    DplEntry *task;
    DplTaskList *tasks;
    DplTaskListIter *iter, *done_iter;
    DplTaskListFilter *done;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1  Task 1\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 1);

    DPL_ASSERT_OK (dpl_tasklist_filter_done (&done));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, done, &done_iter));

    DPL_ASSERT_EQ (dpl_tasklistiter_next (done_iter, &task), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_tasklist_filter_free (done));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (done_iter));
    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
