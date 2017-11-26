#include "dpl_test.h"


int test_unit_list_filter_ref_done_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter_all, *iter_refs, *iter;
    DplEntry *task;
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

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 8);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_filter_type (iter_all,  ENTRY_TASK, &iter_refs));

    DPL_ASSERT_OK (dpl_filter_done (iter_refs, 1, & iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 1"), 0);

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 2"), 0);

    DPL_ASSERT_EQ (dpl_iter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_iter_free (iter_refs));
    DPL_ASSERT_OK (dpl_iter_free (iter_all));

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_filter_type (iter_all,  ENTRY_TASK, &iter_refs));

    DPL_ASSERT_OK (dpl_filter_done (iter_refs,  0, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 3"), 0);

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Task 4"), 0);

    DPL_ASSERT_EQ (dpl_iter_next (iter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_iter_free (iter_refs));
    DPL_ASSERT_OK (dpl_iter_free (iter_all));

    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
