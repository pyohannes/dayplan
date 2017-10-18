#include "dpl_test.h"


int test_unit_list_remove (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter;
    uint32_t len;
    DplEntry *t1, *t2, *t3, *t4;
    DplEntry *curr;

    DPL_ASSERT_OK (dpl_entry_new (&t1, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&t2, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&t3, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&t4, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_list_new (&tasks));

    DPL_ASSERT_OK (dpl_list_push(tasks, t1));
    DPL_ASSERT_OK (dpl_list_push(tasks, t2));
    DPL_ASSERT_OK (dpl_list_push(tasks, t3));
    DPL_ASSERT_OK (dpl_list_push(tasks, t4));
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 4);

    DPL_ASSERT_OK (dpl_list_remove (tasks, t2));
    DPL_ASSERT_OK (dpl_list_remove (tasks, t4));
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 2);

    DPL_ASSERT_OK (dpl_list_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_iter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, t1);

    DPL_ASSERT_OK (dpl_iter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, t3);

    DPL_ASSERT_EQ (dpl_iter_next (iter, &curr), DPL_ITER_END);
    DPL_ASSERT_OK (dpl_iter_free (iter));

    DPL_ASSERT_EQ (dpl_list_remove (tasks, t2), DPL_ERR_NOTFOUND);
    DPL_ASSERT_EQ (dpl_list_remove (tasks, t4), DPL_ERR_NOTFOUND);
    DPL_ASSERT_OK (dpl_list_remove (tasks, t1));
    DPL_ASSERT_OK (dpl_list_remove (tasks, t3));
    DPL_ASSERT_OK (dpl_list_len (tasks, &len));
    DPL_ASSERT_EQ (len, 0);


    DPL_ASSERT_OK (dpl_list_free (tasks, 1));
    DPL_ASSERT_OK (dpl_entry_free (t1));
    DPL_ASSERT_OK (dpl_entry_free (t2));
    DPL_ASSERT_OK (dpl_entry_free (t3));
    DPL_ASSERT_OK (dpl_entry_free (t4));

    return 0;
}
