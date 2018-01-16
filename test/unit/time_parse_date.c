#include "dpl_test.h"


int test_unit_time_parse_date (int argc, char *argv[])
{
    time_t t;
    struct tm tm, tm2;

    DPL_ASSERT_OK (dpl_time_parse_date ("2017-10-09", &tm));
    DPL_ASSERT_EQ (tm.tm_sec, 0)
    DPL_ASSERT_EQ (tm.tm_min, 0)
    DPL_ASSERT_EQ (tm.tm_hour, 0)
    DPL_ASSERT_EQ (tm.tm_mday, 9)
    DPL_ASSERT_EQ (tm.tm_mon, 9)
    DPL_ASSERT_EQ (tm.tm_year, 117)

    DPL_ASSERT_OK (dpl_time_parse_date ("today", &tm));
    t = time (0);
    localtime_r (&t, &tm2);
    DPL_ASSERT_EQ (tm.tm_sec, 0)
    DPL_ASSERT_EQ (tm.tm_min, 0)
    DPL_ASSERT_EQ (tm.tm_hour, 0)
    DPL_ASSERT_EQ (tm.tm_mday, tm2.tm_mday)
    DPL_ASSERT_EQ (tm.tm_mon, tm2.tm_mon)
    DPL_ASSERT_EQ (tm.tm_year, tm2.tm_year)

    DPL_ASSERT_OK (dpl_time_parse_date ("yesterday", &tm));
    t = time (0) - 3600 * 24;
    localtime_r (&t, &tm2);
    DPL_ASSERT_EQ (tm.tm_sec, 0)
    DPL_ASSERT_EQ (tm.tm_min, 0)
    DPL_ASSERT_EQ (tm.tm_hour, 0)
    DPL_ASSERT_EQ (tm.tm_mday, tm2.tm_mday)
    DPL_ASSERT_EQ (tm.tm_mon, tm2.tm_mon)
    DPL_ASSERT_EQ (tm.tm_year, tm2.tm_year)

    DPL_ASSERT_OK (dpl_time_parse_date ("tomorrow", &tm));
    t = time (0) + 3600 * 24;
    localtime_r (&t, &tm2);
    DPL_ASSERT_EQ (tm.tm_sec, 0)
    DPL_ASSERT_EQ (tm.tm_min, 0)
    DPL_ASSERT_EQ (tm.tm_hour, 0)
    DPL_ASSERT_EQ (tm.tm_mday, tm2.tm_mday)
    DPL_ASSERT_EQ (tm.tm_mon, tm2.tm_mon)
    DPL_ASSERT_EQ (tm.tm_year, tm2.tm_year)

    DPL_ASSERT_EQ (dpl_time_parse_date ("invalid", &tm), DPL_ERR_INPUT);

    return 0;
}
