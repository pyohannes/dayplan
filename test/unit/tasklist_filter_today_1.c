#include "dpl_test.h"


int test_unit_tasklist_filter_today_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTaskListIter *fiter;
    DplEntry *task;
    DplTaskListFilter *today;
    const char *title;
    time_t now;
    uint32_t len;
    struct tm tm;
    tm.tm_mday = 10;
    tm.tm_mon = 8;
    tm.tm_year = 117;

    char content[4096]; 

    now = time (0);
    localtime_r (&now, &tm);
    snprintf (content, 4096,
                "2016-09-10\n"
                "  08:00  Projects/Dayplan\n"
                "  12:00  Lunch\n"
                "  13:00  Planning\n"
                "  14:00\n"
                "%d-%02d-%02d\n"
                "  10:00  Writing test plans\n"
                "  12:00  Lunch\n"
                "  13:00  Meetings\n"
                "  16:00\n"
                "2018-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "  09:00  Coffee\n"
                "  11:00\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN, content));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 8);

    DPL_ASSERT_OK (dpl_tasklist_filter_today (&today));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, today, &fiter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Writing test plans"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Meetings"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (fiter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (today));

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
