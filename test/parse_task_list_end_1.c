#include "dpl_test.h"


int test_parse_task_list_end_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTask *task;
    time_t begin;
    time_t end;
    uint32_t len;
    struct tm tm = { 0, 0, 8, 11, 8, 117, 0, 0, 1 };

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "    Wrote a few tests.\n"
                "  09:00  Coffee\n"
                "    Everybody needs a break.\n"
                "  10:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 2);

    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    tm.tm_hour = 9;
    DPL_ASSERT_OK (dpl_task_end_get (task, &end));
    DPL_ASSERT_EQ (end, mktime (&tm));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    tm.tm_hour = 10;
    DPL_ASSERT_OK (dpl_task_end_get (task, &end));
    DPL_ASSERT_EQ (end, mktime (&tm));

    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
