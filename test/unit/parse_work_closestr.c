#include "dpl_test.h"


int test_unit_parse_work_closestr (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter, *iter_all;
    const DplEntry *task;
    int done;
    uint32_t len, id;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Write more tests\n"
                "  #2     Write many more tests\n"
                "  #3     Write many many more tests\n"
                "  08:00  Projects/Dayplan\n"
                "    Fixes #1.\n"
                "  08:30  Projects/Dayplan\n"
                "    closes #2.\n"
                "  09:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 5);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_filter_type (iter_all, ENTRY_TASK, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));

    DPL_ASSERT_OK (dpl_entry_task_id_get (task, &id));
    DPL_ASSERT_OK (dpl_entry_task_done_get (task, &done));
    DPL_ASSERT_OK (id == 3 ? !done : done);

    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_iter_free (iter_all));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
