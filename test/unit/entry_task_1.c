#include "dpl_test.h"


int test_unit_entry_task_1 (int argc, char *argv[])
{
    time_t begin = time (0);
    time_t time_out = 0;
    uint32_t id = 9;
    uint32_t id_out;
    int done = 1;
    int done_out;
    const char *name = "Test/1";
    const char *desc = "Description.";
    const char *string_out;
    DplEntry *task, *task_out;

    DPL_ASSERT_OK (dpl_entry_new (&task, ENTRY_TASK));

    DPL_ASSERT_OK (dpl_entry_name_get (task, &string_out));
    DPL_ASSERT_EQ (string_out, 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &string_out));
    DPL_ASSERT_EQ (string_out, 0);
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &time_out));
    DPL_ASSERT_EQ (time_out, 0);
    DPL_ASSERT_OK (dpl_entry_task_id_get (task, &id_out));
    DPL_ASSERT_EQ (id_out, 0);
    DPL_ASSERT_OK (dpl_entry_task_done_get (task, &task_out));
    DPL_ASSERT_EQ (task_out, 0);

    DPL_ASSERT_EQ (dpl_entry_work_end_get (task, &time_out), DPL_ERR_TYPE);
    DPL_ASSERT_EQ (dpl_entry_work_task_get (task, &task_out), DPL_ERR_TYPE);

    DPL_ASSERT_OK (dpl_entry_name_set (task, name));
    DPL_ASSERT_OK (dpl_entry_desc_set (task, desc));
    DPL_ASSERT_OK (dpl_entry_begin_set (task, begin));
    DPL_ASSERT_OK (dpl_entry_task_id_set (task, id));
    DPL_ASSERT_OK (dpl_entry_task_done_set (task, done));

    DPL_ASSERT_EQ (dpl_entry_work_end_set (task, begin), DPL_ERR_TYPE);
    DPL_ASSERT_EQ (dpl_entry_work_task_set (task, task_out), DPL_ERR_TYPE);

    DPL_ASSERT_OK (dpl_entry_name_get (task, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, name), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (task, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, desc), 0);
    DPL_ASSERT_OK (dpl_entry_begin_get (task, &time_out));
    DPL_ASSERT_EQ (time_out, begin);
    DPL_ASSERT_OK (dpl_entry_task_id_get (task, &id_out));
    DPL_ASSERT_EQ (id_out, id);
    DPL_ASSERT_OK (dpl_entry_task_done_get (task, &done_out));
    DPL_ASSERT_EQ (done_out, done);

    DPL_ASSERT_OK (dpl_entry_free (task));

    return 0;
}
