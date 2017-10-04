#include "dpl_test.h"


int test_unit_parse_error_indent_2 (int argc, char *argv[])
{
    DplTaskList *tasks;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  08:00  Projects/Dayplan\n"
                "    Description\n"
                "      Further invalid indent\n"));

    DPL_ASSERT_EQ (dpl_parse (DPL_tmpfile, &tasks), DPL_ERR_SYNTAX);
    remove (DPL_tmpfile);

    return 0;
}
