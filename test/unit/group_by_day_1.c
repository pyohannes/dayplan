#include "dpl_test.h"


int test_unit_group_by_day_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplGroup *groupfirst, *group;
    DplTaskListIter *iter;
    DplTask *task;
    const char *name;
    const char *title;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-09\n"
                "  08:00  Projects/Dayplan\n"
                "  12:00  Lunch\n"
                "  13:00\n"
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
    DPL_ASSERT_EQ (len, 7);

    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_group_by_day (iter, &group));
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    groupfirst = group;

    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "2017-09-09"), 0);
    DPL_ASSERT_OK (dpl_group_tasks_get (group, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);
    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "2017-09-10"), 0);
    DPL_ASSERT_OK (dpl_group_tasks_get (group, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Planning"), 0);
    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "2017-09-11"), 0);
    DPL_ASSERT_OK (dpl_group_tasks_get (group, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);
    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));

    DPL_ASSERT_EQ (dpl_group_next (group, &group), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
