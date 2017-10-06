#include "dpl_test.h"


int test_unit_parse_error_indent_1 (int argc, char *argv[])
{
    DplTaskList *tasks;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                " no full deindent\n"));

    DPL_ASSERT_EQ (dpl_parse (DPL_tmpfile, &tasks, 0), DPL_ERR_SYNTAX);
    remove (DPL_tmpfile);

    return 0;
}
