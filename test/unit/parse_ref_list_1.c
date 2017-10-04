#include "dpl_test.h"


int test_unit_parse_ref_list_1 (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    DplTask *task;
    DplRef *ref;
    DplTaskType type;
    time_t begin;
    const char *title;
    const char *desc;
    uint32_t len;
    struct tm tm = { 0, 0, 0, 11, 8, 117, 0, 0, 1 };

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Projects/Dayplan\n"
                "    Write some tests.\n"
                "  08:00  #1\n"
                "  09:00  Coffee\n"
                "    Everybody needs a break.\n"
                "\n"
                "  10:00  #1\n"
                "    Back to work.\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 4);

    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_type_get (task, &type));
    DPL_ASSERT_EQ (type, ref_type);
    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_task_end_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_task_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);
    DPL_ASSERT_OK (dpl_task_ref_get (task, &ref));
    DPL_ASSERT_OK (dpl_ref_title_get (ref, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_ref_desc_get (ref, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Write some tests."), 0);

    tm.tm_hour = 8;
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_type_get (task, &type));
    DPL_ASSERT_EQ (type, task_type);
    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_task_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);
    DPL_ASSERT_OK (dpl_task_ref_get (task, &ref));
    DPL_ASSERT_OK (dpl_ref_desc_get (ref, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Write some tests."), 0);

    tm.tm_hour = 9;
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_type_get (task, &type));
    DPL_ASSERT_EQ (type, task_type);
    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);
    DPL_ASSERT_OK (dpl_task_desc_get (task, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Everybody needs a break."), 0);

    tm.tm_hour = 10;
    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &task));
    DPL_ASSERT_OK (dpl_task_type_get (task, &type));
    DPL_ASSERT_EQ (type, task_type);
    DPL_ASSERT_OK (dpl_task_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_task_title_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_task_desc_get (task, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Back to work."), 0);

    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
