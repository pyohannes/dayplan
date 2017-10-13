#include "dpl_test.h"


int test_unit_entry_work_1 (int argc, char *argv[])
{
    time_t begin_in = time (0);
    time_t time_out = 0;
    uint32_t id_out;
    int done_out;
    const char *name = "Test/1";
    const char *desc = "Description.";
    const char *string_out;
    DplEntry *work, *task_out;

    DPL_ASSERT_OK (dpl_entry_new (&work, ENTRY_WORK));

    DPL_ASSERT_OK (dpl_entry_name_get (work, &string_out));
    DPL_ASSERT_EQ (string_out, 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (work, &string_out));
    DPL_ASSERT_EQ (string_out, 0);
    DPL_ASSERT_OK (dpl_entry_begin_get (work, &time_out));
    DPL_ASSERT_EQ (time_out, 0);
    DPL_ASSERT_OK (dpl_entry_work_end_get (work, &time_out));
    DPL_ASSERT_EQ (time_out, 0);
    DPL_ASSERT_OK (dpl_entry_work_task_get (work, &task_out));
    DPL_ASSERT_EQ (task_out, 0);

    DPL_ASSERT_EQ (dpl_entry_task_id_get (work, &id_out), DPL_ERR_TYPE);
    DPL_ASSERT_EQ (dpl_entry_task_done_get (work, &done_out), DPL_ERR_TYPE);

    DPL_ASSERT_OK (dpl_entry_name_set (work, name));
    DPL_ASSERT_OK (dpl_entry_desc_set (work, desc));
    DPL_ASSERT_OK (dpl_entry_begin_set (work, begin_in));
    DPL_ASSERT_OK (dpl_entry_work_end_set (work, begin_in + 3600));

    DPL_ASSERT_EQ (dpl_entry_task_id_set (work, id_out), DPL_ERR_TYPE);
    DPL_ASSERT_EQ (dpl_entry_task_done_set (work, done_out), DPL_ERR_TYPE);

    DPL_ASSERT_OK (dpl_entry_name_get (work, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, name), 0);
    DPL_ASSERT_OK (dpl_entry_desc_get (work, &string_out));
    DPL_ASSERT_EQ (strcmp (string_out, desc), 0);
    DPL_ASSERT_OK (dpl_entry_begin_get (work, &time_out));
    DPL_ASSERT_EQ (time_out, begin_in);
    DPL_ASSERT_OK (dpl_entry_work_end_get (work, &time_out));
    DPL_ASSERT_EQ (time_out, begin_in + 3600);
    DPL_ASSERT_OK (dpl_entry_work_task_get (work, &task_out));
    DPL_ASSERT_EQ (task_out, 0);

    DPL_ASSERT_OK (dpl_entry_free (work));

    return 0;
}
