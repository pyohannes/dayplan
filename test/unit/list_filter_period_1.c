#include "dpl_test.h"


int test_unit_list_filter_period_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter;
    DplIter *fiter;
    const DplEntry *task;
    time_t begin;
    time_t end;
    const char *title;
    uint32_t len;
    struct tm tm;
    tm.tm_sec = 0; 
    tm.tm_min = 0; 
    tm.tm_hour = 0; 
    tm.tm_mday = 10; 
    tm.tm_mon = 8; 
    tm.tm_year = 117; 

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-10\n"
                "  08:00  Projects/Dayplan\n"
                "  12:00  Lunch\n"
                "  13:00  Planning\n"
                "  14:00\n"
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "  09:00  Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse (DPL_tmpfile, &tasks, 0));
    DPL_ASSERT_NEQ (tasks, 0);
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 5);

    begin = mktime (&tm);
    tm.tm_mday += 1;
    end = mktime (&tm);
    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_filter_period (iter, begin, end, &fiter));

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Planning"), 0);

    DPL_ASSERT_EQ (dpl_iter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (fiter));
    DPL_ASSERT_OK (dpl_iter_free (iter));

    begin = end;
    tm.tm_mday += 1;
    end = mktime (&tm);
    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));
    DPL_ASSERT_OK (dpl_filter_period (iter, begin, end, &fiter));

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);

    DPL_ASSERT_OK (dpl_iter_next (fiter, &task));
    DPL_ASSERT_OK (dpl_entry_name_get (task, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);

    DPL_ASSERT_EQ (dpl_iter_next (fiter, &task), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (fiter));
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    remove (DPL_tmpfile);

    return 0;
}
