#include "dpl_test.h"


int test_unit_tasklist_filter_task_ref_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTaskListIter *fiter;
    DplTask *task;
    DplRef *ref;
    DplTaskListFilter *filter;
    time_t begin;
    time_t end;
    const char *title;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Projects/Dayplan\n"
                "  08:00  #1\n"
                "  09:00  Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 3);

    DPL_ASSERT_OK (dpl_tasklist_filter_tasks (&filter));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, filter, &fiter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (fiter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (filter));

    DPL_ASSERT_OK (dpl_tasklist_filter_refs (&filter));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter, filter, &fiter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_task_ref_get (task, &ref));
    DPL_ASSERT_OK (dpl_ref_title_get (ref, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (fiter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_filter_free (filter));

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
