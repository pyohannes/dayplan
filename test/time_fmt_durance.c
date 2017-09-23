#include "dpl_test.h"


int test_time_fmt_durance (int argc, char *argv[])
{
#define BUFSIZE 1024
    char fmt[BUFSIZE];

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%Ss", 0));
    DPL_ASSERT_EQ (strcmp (fmt, "0s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%Ssecs", 53));
    DPL_ASSERT_EQ (strcmp (fmt, "53secs"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%mm %ss", 60));
    DPL_ASSERT_EQ (strcmp (fmt, " 1m  0s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%mm %ss", 61));
    DPL_ASSERT_EQ (strcmp (fmt, " 1m  1s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%m m%ss", 3050));
    DPL_ASSERT_EQ (strcmp (fmt, "50 m50s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%hh %mm %ss", 3600));
    DPL_ASSERT_EQ (strcmp (fmt, " 1h  0m  0s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%hh %mm %ss", 6650));
    DPL_ASSERT_EQ (strcmp (fmt, " 1h 50m 50s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "In %Hhs%mms%sss", 86400));
    DPL_ASSERT_EQ (strcmp (fmt, "In 24hs 0ms 0ss"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%Hh %mm %ss", 93050));
    DPL_ASSERT_EQ (strcmp (fmt, "25h 50m 50s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%dd %hh %mm %ss", 93050));
    DPL_ASSERT_EQ (strcmp (fmt, "1d  1h 50m 50s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%Mm %ss", 93050));
    DPL_ASSERT_EQ (strcmp (fmt, "1550m 50s"), 0);

    DPL_ASSERT_OK (dpl_time_fmt_durance (fmt, BUFSIZE, "%Ss", 93050));
    DPL_ASSERT_EQ (strcmp (fmt, "93050s"), 0);

    return 0;
}
