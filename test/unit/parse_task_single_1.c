#include "dpl_test.h"


int test_unit_parse_task_single_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTask *task;
    time_t begin;
    time_t end;
    const char *title;
    const char *desc;
    uint32_t len;
    struct tm tm_begin = { 0, 0, 8, 11, 8, 117, 0, 0, 1 };

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "  09:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 1);

    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));

    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm_begin));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_task_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);

    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
