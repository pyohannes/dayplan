#include "dpl_test.h"


int test_unit_parse_category (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter, *iter_all;
    const DplEntry *work, *task;
    uint32_t len, id;
    const char *cat;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     (testing) Write more tests\n"
                "  08:00  #1\n"
                "  09:00  (coding) #1\n"
                "  10:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 3);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter_all));
    DPL_ASSERT_OK (dpl_filter_type (iter_all, ENTRY_WORK, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &work));

    DPL_ASSERT_OK (dpl_entry_work_task_get (work, &task));
    DPL_ASSERT_OK (task != 0);
    DPL_ASSERT_OK (dpl_entry_task_id_get (task, &id));
    DPL_ASSERT_EQ (id, 1);
    DPL_ASSERT_OK (dpl_entry_category_get (task, &cat));
    DPL_ASSERT_EQ (strcmp (cat, "testing"), 0)
    DPL_ASSERT_OK (dpl_entry_category_get (work, &cat));
    DPL_ASSERT_EQ (strcmp (cat, "testing"), 0)

    DPL_ASSERT_OK (dpl_iter_next (iter, &work));

    DPL_ASSERT_OK (dpl_entry_work_task_get (work, &task));
    DPL_ASSERT_OK (task != 0);
    DPL_ASSERT_OK (dpl_entry_task_id_get (task, &id));
    DPL_ASSERT_EQ (id, 1);
    DPL_ASSERT_OK (dpl_entry_category_get (task, &cat));
    DPL_ASSERT_EQ (strcmp (cat, "testing"), 0)
    DPL_ASSERT_OK (dpl_entry_category_get (work, &cat));
    DPL_ASSERT_EQ (strcmp (cat, "coding"), 0)

    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_iter_free (iter_all));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
