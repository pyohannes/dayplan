#include "dpl_test.h"


int test_unit_entry_work_2 (int argc, char *argv[])
{
    const char *taskname = "Task";
    const char *workname = "Work";
    const char *string_out;
    DplEntry *work, *task;

    DPL_ASSERT_OK (dpl_entry_new (&task, ENTRY_TASK));
    DPL_ASSERT_OK (dpl_entry_name_set (task, taskname));

    DPL_ASSERT_OK (dpl_entry_new (&work, ENTRY_WORK));
    DPL_ASSERT_OK (dpl_entry_work_task_set (work, task));

    DPL_ASSERT_OK (dpl_entry_name_get (work, &string_out));
    DPL_ASSERT_EQ (string_out, 0);

    DPL_ASSERT_OK (dpl_entry_name_set (work, workname));
    DPL_ASSERT_OK (dpl_entry_name_get (work, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, workname), 0);

    DPL_ASSERT_OK (dpl_entry_free (work));

    return 0;
}
