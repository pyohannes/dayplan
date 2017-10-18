#include "dpl_test.h"


int test_unit_parse_date_space_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter;
    DplEntry *task;
    DplEntryType type;
    const char *title;
    const char *desc;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "\n\n"
                "  #1     Projects/Dayplan\n"
                "    Write some tests.\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 1);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_type_get (task, &type));
    DPL_ASSERT_EQ (type, ENTRY_TASK);
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Write some tests."), 0);

    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
