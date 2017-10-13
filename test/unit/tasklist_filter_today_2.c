#include "dpl_test.h"


int test_unit_tasklist_filter_today_2 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTaskListIter *fiter;
    DplEntry *task;
    DplTaskListFilter *today;
    const char *title;
    time_t now;
    uint32_t len;
    struct tm tm_today, tm_tomorrow;

    char content[4096]; 

    now = time (0);
    localtime_r (&now, &tm_today);
    now += 3600 * 24;
    localtime_r (&now, &tm_tomorrow);
    snprintf (content, 4096,
                "%d-%02d-%02d\n"
                "  #1     A\n"
                "  #2     B\n"
                "%d-%02d-%02d\n"
                "  #3     C\n"
                "  #4     C\n"
                "  8:00\n",
                tm_today.tm_year + 1900, tm_today.tm_mon + 1, tm_today.tm_mday,
                tm_tomorrow.tm_year + 1900, tm_tomorrow.tm_mon + 1, 
                tm_tomorrow.tm_mday);
    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN, content));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 6);

    DPL_ASSERT_OK (dpl_tasklist_filter_today (&today));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, today, &fiter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "A"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "B"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (fiter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (today));

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
