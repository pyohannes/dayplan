#include "dpl_test.h"


int test_unit_filter_task_for_work_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter_all, *iter_works, *iter_tasks;
    DplEntry *entry;
    const char *name;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-10\n"
                "  #1 Task 1\n"
                "  #2 Task 2\n"
                "  #3 Task 3\n"
                "  #4 Task 4\n"
                "  08:00  #2\n"
                "    For #2\n"
                "  09:00  #4\n"
                "    And #4\n"
                "  10:00  #1\n"
                "    This for #1\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 7);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_filter_type (iter_all, ENTRY_WORK, &iter_works));

    DPL_ASSERT_OK (dpl_filter_task_for_work (iter_works, &iter_tasks));

    DPL_ASSERT_OK (dpl_iter_next (iter_tasks, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Task 2"), 0);

    DPL_ASSERT_OK (dpl_iter_next (iter_tasks, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Task 4"), 0);

    DPL_ASSERT_OK (dpl_iter_next (iter_tasks, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Task 1"), 0);

    DPL_ASSERT_EQ (dpl_iter_next (iter_tasks, &entry), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_iter_free (iter_all));
    DPL_ASSERT_OK (dpl_iter_free (iter_works));
    DPL_ASSERT_OK (dpl_iter_free (iter_tasks));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));

    remove (DPL_tmpfile);

    return 0;
}
