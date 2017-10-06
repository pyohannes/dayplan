#include "dpl_test.h"


int test_unit_tasklist_filter_ref_done_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter_all, *iter_refs, *iter;
    DplTask *task;
    DplTaskListFilter *f_refs, *f_done, *f_undone;
    const char *title;
    uint32_t len;

    char content[4096]; 

    snprintf (content, 4096,
                "2016-09-10\n"
                "  #1  Task 1\n"
                "  #2  Task 2\n"
                "  #3  Task 3\n"
                "  #4  Task 4\n"
                "  08:00  #1\n"
                "  09:00  +#2\n"
                "  10:00  +#1\n"
                "  11:00  #3\n"
                "  12:00\n");
    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN, content));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 8);

    DPL_ASSERT_OK (dpl_tasklist_filter_refs (&f_refs));
    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter_all, f_refs, &iter_refs));

    DPL_ASSERT_OK (dpl_tasklist_filter_done (&f_done));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter_refs, f_done, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 1"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 2"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklist_filter_free (f_done));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter_refs));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter_all));

    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter_all, f_refs, &iter_refs));

    DPL_ASSERT_OK (dpl_tasklist_filter_undone (&f_undone));
    DPL_ASSERT_OK (dpl_tasklist_filter (iter_refs, f_undone, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 3"), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 4"), 0);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklist_filter_free (f_undone));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter_refs));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter_all));

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
