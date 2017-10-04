#include "dpl_test.h"


int test_unit_ref_title (int argc, char *argv[])
{
    const char *title_in = "Projects/Dayplan/Codeing";
    const char *title_out = 0;
    DplRef *r;

    DPL_ASSERT_OK (dpl_ref_new (&r));
    DPL_ASSERT_OK (dpl_ref_title_set (r, title_in));
    DPL_ASSERT_OK (dpl_ref_title_get (r, &title_out));
    DPL_ASSERT_EQ (strcmp (title_in, title_out), 0);

    DPL_ASSERT_OK (dpl_ref_free (r));

    return 0;
}
