#include "dpl_test.h"


int test_unit_ref_desc (int argc, char *argv[])
{
    const char *desc_in = "This is a multiline\ndescription.";
    const char *desc_out = 0;
    DplRef *r;

    DPL_ASSERT_OK (dpl_ref_new (&r));
    DPL_ASSERT_OK (dpl_ref_desc_set (r, desc_in));
    DPL_ASSERT_OK (dpl_ref_desc_get (r, &desc_out));
    DPL_ASSERT_EQ (strcmp (desc_in, desc_out), 0);

    DPL_ASSERT_OK (dpl_ref_free (r));

    return 0;
}
