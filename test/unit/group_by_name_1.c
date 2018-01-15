#include "dpl_test.h"


int test_unit_group_by_name_1 (int argc, char *argv[])
{
    DplList *entries;
    DplGroup *group;
    DplIter *iter;
    const DplEntry *entry;
    const char *name;
    uint32_t len;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-09\n"
                "  08:00  Projects/Dayplan/Test\n"
                "  12:00  Break/Lunch\n"
                "  13:00\n"
                "2017-09-10\n"
                "  08:00  Projects/Dayplan/Implementation\n"
                "  12:00  Break/Lunch\n"
                "  13:00  General/Planning\n"
                "  14:00\n"
                "2017-09-11\n"
                "  08:00  Projects/Dayplan/Test\n"
                "  09:00  Break/Coffee\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &entries, 0));
    DPL_ASSERT_NEQ (entries, 0);
    DPL_ASSERT_OK (dpl_list_len (entries, &len));
    DPL_ASSERT_EQ (len, 7);

    DPL_ASSERT_OK (dpl_list_iter (entries, &iter));
    DPL_ASSERT_OK (dpl_group_by_name (iter, &group));
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "Break/Coffee"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Break/Coffee"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));


    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "Break/Lunch"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Break/Lunch"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Break/Lunch"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "General/Planning"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "General/Planning"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "Projects/Dayplan/Implementation"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Projects/Dayplan/Implementation"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "Projects/Dayplan/Test"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Projects/Dayplan/Test"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_name_get (entry, &name));
    DPL_ASSERT_EQ (strcmp (name, "Projects/Dayplan/Test"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_EQ (dpl_group_next (group, &group), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_list_free (entries, 1));
    remove (DPL_tmpfile);

    return 0;
}
