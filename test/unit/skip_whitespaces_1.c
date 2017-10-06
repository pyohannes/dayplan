#include "dpl_test.h"
#include "dpl/utils.h"


int test_unit_skip_whitespaces_1 (int argc, char *argv[])
{
    char *s1 = "This string\nshould stay\tunchanged.";
    char *s1_stripped = "This string\nshould stay\tunchanged.";

    char *s2 = "   White spaces should be stripped from the begining.";
    char *s2_stripped = "White spaces should be stripped from the begining.";

    char *s3 = "Line breaks and spaces from the end. \n\n";
    char *s3_stripped = "Line breaks and spaces from the end.";

    char *s4 = " Strip from the beginning and from the end.\n ";
    char *s4_stripped = "Strip from the beginning and from the end.";

    DPL_ASSERT_EQ (dpl_skip_whitespaces (s1), s1);
    DPL_ASSERT_EQ (strcmp (dpl_skip_whitespaces (s1), s1_stripped), 0);

    DPL_ASSERT_EQ (strcmp (dpl_skip_whitespaces (s2), s2_stripped), 0);
    DPL_ASSERT_EQ (strcmp (dpl_skip_whitespaces (s3), s3_stripped), 0);
    DPL_ASSERT_EQ (strcmp (dpl_skip_whitespaces (s4), s4_stripped), 0);

    return 0;
}
