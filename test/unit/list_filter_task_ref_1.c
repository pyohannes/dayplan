#include "dpl_test.h"


int test_unit_list_filter_task_ref_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter;
    DplIter *fiter;
    DplEntry *task;
    const DplEntry *ref;
    time_t begin;
    time_t end;
    const char *title;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Write tests.\n"
                "  08:00  Projects/Dayplan\n"
                "    Relates to #1.\n"
                "  09:00  Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 3);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_filter_type (iter, ENTRY_WORK, &fiter));

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);

    DPL_ASSERT_EQ (dpl_iter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (fiter));
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_filter_type (iter, ENTRY_TASK, &fiter));

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Write tests."), 0);

    DPL_ASSERT_EQ (dpl_iter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (fiter));
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
