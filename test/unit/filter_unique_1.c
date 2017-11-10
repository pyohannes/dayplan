#include "dpl_test.h"


int test_unit_filter_unique_1 (int argc, char *argv[])
{
    DplList *entries;
    DplIter *iter_all, *iter_unique;
    DplEntry *entry, *entry1, *entry2, *entry3;

    DPL_ASSERT_OK (dpl_entry_new (&entry1, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&entry2, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&entry3, ENTRY_WORK));

    DPL_ASSERT_OK (dpl_list_new (&entries));

    DPL_ASSERT_OK (dpl_list_push (entries, entry1));
    DPL_ASSERT_OK (dpl_list_push (entries, entry1));
    DPL_ASSERT_OK (dpl_list_push (entries, entry2));
    DPL_ASSERT_OK (dpl_list_push (entries, entry3));
    DPL_ASSERT_OK (dpl_list_push (entries, entry1));
    DPL_ASSERT_OK (dpl_list_push (entries, entry3));
    DPL_ASSERT_OK (dpl_list_push (entries, entry2));
    DPL_ASSERT_OK (dpl_list_push (entries, entry1));
    DPL_ASSERT_OK (dpl_list_push (entries, entry3));
    DPL_ASSERT_OK (dpl_list_push (entries, entry2));


    DPL_ASSERT_OK (dpl_list_iter (entries, &iter_all));
    DPL_ASSERT_OK (dpl_filter_unique (iter_all, &iter_unique));

    DPL_ASSERT_OK (dpl_iter_next (iter_unique, &entry));
    DPL_ASSERT_EQ (entry, entry1);

    DPL_ASSERT_OK (dpl_iter_next (iter_unique, &entry));
    DPL_ASSERT_EQ (entry, entry2);

    DPL_ASSERT_OK (dpl_iter_next (iter_unique, &entry));
    DPL_ASSERT_EQ (entry, entry3);

    DPL_ASSERT_EQ (dpl_iter_next (iter_unique, &entry), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_iter_free (iter_all));
    DPL_ASSERT_OK (dpl_iter_free (iter_unique));
    DPL_ASSERT_OK (dpl_list_free (entries, 0));
    DPL_ASSERT_OK (dpl_entry_free (entry1));
    DPL_ASSERT_OK (dpl_entry_free (entry2));
    DPL_ASSERT_OK (dpl_entry_free (entry3));

    remove (DPL_tmpfile);

    return 0;
}
