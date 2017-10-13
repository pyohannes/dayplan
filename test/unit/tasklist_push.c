#include "dpl_test.h"


int test_unit_tasklist_push (int argc, char *argv[])
{
    DplTaskList *tasks;
    DplTaskListIter *iter;
    uint32_t len;
    DplEntry *t1, *t2, *t3;
    DplEntry *curr;

    DPL_ASSERT_OK (dpl_entry_new (&t1, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&t2, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_new (&t3, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_tasklist_new (&tasks));

    DPL_ASSERT_OK (dpl_tasklist_push(tasks, t1));
    DPL_ASSERT_OK (dpl_tasklist_push(tasks, t2));
    DPL_ASSERT_OK (dpl_tasklist_push(tasks, t3));
    DPL_ASSERT_OK (dpl_tasklist_len (tasks, &len));
    DPL_ASSERT_EQ (len, 3);

    DPL_ASSERT_OK (dpl_tasklist_iter (tasks, &iter));

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, t1);

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, t2);

    DPL_ASSERT_OK (dpl_tasklistiter_next (iter, &curr));
    DPL_ASSERT_EQ (curr, t3);

    DPL_ASSERT_EQ (dpl_tasklistiter_next (iter, &curr), DPL_ITER_END);

    DPL_ASSERT_OK (dpl_tasklistiter_free (iter));
    DPL_ASSERT_OK (dpl_tasklist_free (tasks, 1));

    return 0;
}
