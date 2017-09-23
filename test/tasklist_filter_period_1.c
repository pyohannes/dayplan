#include "dpl_test.h"


int test_tasklist_filter_period_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTaskListIter *fiter;
    DplTask *task;
    DplTaskListFilter *period;
    time_t begin;
    time_t end;
    const char *title;
    uint32_t len;
    struct tm tm = { 0, 0, 0, 10, 8, 117, 0, 0, 1 };

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-10\n"
                "  08:00  Projects/Dayplan\n"
                "  12:00  Lunch\n"
                "  13:00  Planning\n"
                "  14:00\n"
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "  09:00  Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 5);

    begin = mktime (&tm);
    tm.tm_mday += 1;
    end = mktime (&tm);
    DPL_ASSERT_OK (dpl_tasklist_filter_period (begin, end, &period));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, period, &fiter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Planning"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (fiter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (period));

    begin = end;
    tm.tm_mday += 1;
    end = mktime (&tm);
    DPL_ASSERT_OK (dpl_tasklist_filter_period (begin, end, &period));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, period, &fiter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (fiter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (period));

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
