#include "dpl_test.h"


int test_unit_tasklist_filter_task_ref_2 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter_ref, *iter_all, *iter;
    DplTaskListFilter *f_refs, *f_date;
    DplEntry *task;
    const DplEntry *ref;
    const char *title;
    uint32_t len;
    struct tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 11;
    tm.tm_mon = 8;
    tm.tm_year = 117;
    tm.tm_isdst = 1;

    time_t begin = mktime (&tm);
    time_t end = begin + 3600;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-10\n"
                "  #1     Projects/Dayplan\n"
                "  08:00  #1\n"
                "  09:00  Coffee\n"
                "  10:00\n"
                "2017-09-11\n"
                "  08:00  #1\n"
                "  09:00  Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 6);

    DPL_ASSERT_OK (dpl_tasklist_filter_refs (&f_refs));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter_all, f_refs, &iter_ref));

    DPL_ASSERT_OK (dpl_tasklist_filter_period (begin, end, &f_date));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter_ref, f_date, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter_all));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter_ref));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (f_date));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (f_refs));

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
