#include "dpl_test.h"


int test_unit_group_by_day_1 (int argc, char *argv[])
{
    DplList *entries;
    DplGroup *groupfirst, *group;
    DplIter *iter;
    DplEntry *entry;
    const char *name;
    const char *title;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-09\n"
                "  08:00  Projects/Dayplan\n"
                "  12:00  Lunch\n"
                "  13:00\n"
                "2017-09-10\n"
                "  08:00  Projects/Dayplan\n"
                "  12:00  Lunch\n"
                "  13:00  Planning\n"
                "  14:00\n"
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "  09:00  Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &entries, 0));
    DPL_ASSERT_NEQ (entries, 0);
    DPL_ASSERT_OK (dpl_list_len (entries, &len));
    DPL_ASSERT_EQ (len, 7);

    DPL_ASSERT_OK (dpl_list_iter (entries, &iter));
    DPL_ASSERT_OK (dpl_group_by_day (iter, &group));
    DPL_ASSERT_OK (dpl_iter_free (iter));
    groupfirst = group;

    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "2017-09-09"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "2017-09-10"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Lunch"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Planning"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "2017-09-11"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Projects/Dayplan"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "Coffee"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_EQ (dpl_group_next (group, &group), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_list_free (entries, 1));
    remove (DPL_tmpfile);

    return 0;
}
