#include "dpl_test.h"


int test_unit_group_by_category_1 (int argc, char *argv[])
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
                "  08:00  (projects/dayplan/test) Write tests\n"
                "  12:00  (break/lunch) Soup\n"
                "  13:00\n"
                "2017-09-10\n"
                "  08:00  (projects/dayplan/implementation) Fix issue 9\n"
                "  12:00  (break/lunch) Sandwich\n"
                "  13:00  (general/planning) Plan next week\n"
                "  14:00\n"
                "2017-09-11\n"
                "  08:00  (projects/dayplan/test) Write more tests\n"
                "  09:00  (break/coffee) Mmmh ...\n"
                "  11:00\n"));

    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &entries, 0));
    DPL_ASSERT_NEQ (entries, 0);
    DPL_ASSERT_OK (dpl_list_len (entries, &len));
    DPL_ASSERT_EQ (len, 7);

    DPL_ASSERT_OK (dpl_list_iter (entries, &iter));
    DPL_ASSERT_OK (dpl_group_by_category (iter, &group));
    DPL_ASSERT_OK (dpl_iter_free (iter));
    groupfirst = group;

    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "break"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "break/lunch"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "break/lunch"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "break/coffee"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "break/coffee"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "break/coffee"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "break/lunch"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "break/lunch"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "break/lunch"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "general"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "general/planning"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "general/planning"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "general/planning"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "projects"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/test"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/implementation"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/test"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "projects/dayplan"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/test"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/implementation"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/test"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "projects/dayplan/implementation"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/implementation"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_group_next (group, &group));
    DPL_ASSERT_OK (dpl_group_name_get (group, &name));
    DPL_ASSERT_EQ (strcmp (name, "projects/dayplan/test"), 0);
    DPL_ASSERT_OK (dpl_group_entries_get (group, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/test"), 0);
    DPL_ASSERT_OK (dpl_iter_next (iter, &entry));
    DPL_ASSERT_OK (dpl_entry_category_get (entry, &title));
    DPL_ASSERT_EQ (strcmp (title, "projects/dayplan/test"), 0);
    DPL_ASSERT_EQ (dpl_iter_next (iter, &entry), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_EQ (dpl_group_next (group, &group), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_list_free (entries, 1));
    remove (DPL_tmpfile);

    return 0;
}
