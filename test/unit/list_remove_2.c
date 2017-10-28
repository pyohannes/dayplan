#include "dpl_test.h"


int test_unit_list_remove_2 (int argc, char *argv[])
{
    DplList *entries;
    DplIter *iter;
    DplEntry *e1, *e2, *e3;
    uint32_t len;
    const DplEntry *curr;

    DPL_ASSERT_OK (dpl_entry_new (&e1, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&e2, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&e3, ENTRY_WORK));

    DPL_ASSERT_OK (dpl_list_new (&entries));

    DPL_ASSERT_OK (dpl_list_push (entries, e1));
    DPL_ASSERT_OK (dpl_list_push (entries, e2));
    DPL_ASSERT_OK (dpl_list_remove (entries, e2));
    DPL_ASSERT_OK (dpl_list_push (entries, e3));

    DPL_ASSERT_OK (dpl_list_len (entries, &len));
    DPL_ASSERT_EQ (len, 2);

    DPL_ASSERT_OK (dpl_list_iter (entries, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, e1);

    DPL_ASSERT_OK (dpl_iter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, e3);

    DPL_ASSERT_EQ (dpl_iter_next (iter, &curr), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_OK (dpl_list_free (entries, 1));
    DPL_ASSERT_OK (dpl_entry_free (e2));

    return 0;
}
