#include "dpl_test.h"


int test_unit_parse_work_refstr_1 (int argc, char *argv[])
{
    DplList *tasks;
    DplIter *iter, *iter_all;
    const DplEntry *work, *task;
    uint32_t len, id;

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Write more tests\n"
                "  #2     Fix bugs\n"
                "  08:00  #1 #2\n"
                "  09:00\n"));
    DPL_ASSERT_EQ (dpl_parse_file (DPL_tmpfile, &tasks, 1), DPL_ERR_INPUT);

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Write more tests\n"
                "  #2     Fix bugs\n"
                "  08:00  Work\n"
                "    #1 and #2\n"
                "  09:00\n"));
    DPL_ASSERT_EQ (dpl_parse_file (DPL_tmpfile, &tasks, 1), DPL_ERR_INPUT);

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Write more tests\n"
                "  #2     Fix bugs\n"
                "  08:00  #1\n"
                "    #2\n"
                "  09:00\n"));
    DPL_ASSERT_EQ (dpl_parse_file (DPL_tmpfile, &tasks, 1), DPL_ERR_INPUT);

    DPL_ASSERT_OK (dpl_test_write (DPL_tmpfile, DPL_TMPFILE_LEN,
                "2017-09-11\n"
                "  #1     Write more tests\n"
                "  #2     Fix bugs\n"
                "  08:00  #1\n"
                "    #1\n"
                "  09:00\n"));
    DPL_ASSERT_OK (dpl_parse_file (DPL_tmpfile, &tasks, 1));

    remove (DPL_tmpfile);

    return 0;
}
