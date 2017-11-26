#include "dpl_test.h"


int test_unit_parse_task_list_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter;
    DplEntry *task;
    time_t begin;
    time_t end;
    const char *title;
    const char *desc;
    uint32_t len;
    struct tm tm_begin;
    tm_begin.tm_sec = 0;
    tm_begin.tm_min = 0;
    tm_begin.tm_hour = 8;
    tm_begin.tm_mday = 11;
    tm_begin.tm_mon = 8;
    tm_begin.tm_year = 117;

    tzset ();
    tm_begin.tm_isdst = -1;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "  09:00  Coffee\n"
                "  10:00  Projects/Dayplan\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 3);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm_begin));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);

    tm_begin.tm_hour = 9;
    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm_begin));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);

    tm_begin.tm_hour = 10;
    DPL_ASSERT_OK (dpl_iter_next (iter, &task));
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &begin));
    DPL_ASSERT_EQ (begin, mktime (&tm_begin));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &desc));
    DPL_ASSERT_EQ (desc, 0);

    DPL_ASSERT_OK (dpl_iter_free (iter));
    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
