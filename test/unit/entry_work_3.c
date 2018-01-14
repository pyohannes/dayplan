#include "dpl_test.h"


int test_unit_entry_work_3 (int argc, char *argv[])
{
    const char *taskcat = "task";
    const char *workcat = "work";
    const char *string_out;
    DplEntry *work, *task;

    DPL_ASSERT_OK (dpl_entry_new (&task, ENTRY_TASK));
    DPL_ASSERT_OK (dpl_entry_category_set (task, taskcat));

    DPL_ASSERT_OK (dpl_entry_new (&work, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_work_task_set (work, task));

    DPL_ASSERT_OK (dpl_entry_category_get (work, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, taskcat), 0);

    DPL_ASSERT_OK (dpl_entry_category_set (work, workcat));
    DPL_ASSERT_OK (dpl_entry_category_get (work, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, workcat), 0);

    DPL_ASSERT_OK (dpl_entry_free (work));

    return 0;
}
