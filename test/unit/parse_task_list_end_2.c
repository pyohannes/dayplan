#include "dpl_test.h"


int test_unit_parse_task_list_end_2 (int argc, char *argv[])
{
    DplList *tasks;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "    Wrote a few tests.\n"
                "  09:00  Coffee\n"
                "    Everybody needs a break.\n"));

    DPL_ASSERT_EQ (dpl_parse (DPL_tmpfile, &tasks, 0), DPL_ERR_INPUT);

    remove (DPL_tmpfile);

    return 0;
}
