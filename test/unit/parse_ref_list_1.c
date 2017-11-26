#include "dpl_test.h"


int test_unit_parse_ref_list_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter;
    DplEntry *task;
    const DplEntry *ref;
    DplEntryType type;
    time_t begin;
    const char *title;
    const char *desc;
    uint32_t len;
    struct tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 11;
    tm.tm_mon = 8;
    tm.tm_year = 117;

    tzset ();
    tm.tm_isdst = -1;

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

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 4);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_type_get (task, &type));
    DPL_ASSERT_EQ (type, ENTRY_TASK);
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Write some tests."), 0);

    tm.tm_hour = 8;
    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_type_get (task, &type));
    DPL_ASSERT_EQ (type, ENTRY_WORK);
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);
    DPL_ASSERT_OK (dpl_entry_work_task_get (task, &ref));
    DPL_ASSERT_OK (dpl_entry_desc_get (ref, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Write some tests."), 0);

    tm.tm_hour = 9;
    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_type_get (task, &type));
    DPL_ASSERT_EQ (type, ENTRY_WORK);
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Everybody needs a break."), 0);

    tm.tm_hour = 10;
    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_type_get (task, &type));
    DPL_ASSERT_EQ (type, ENTRY_WORK);
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (strcmp (desc, "Back to work."), 0);

    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
