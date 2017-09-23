#include "dpl_test.h"


int test_ref_id (int argc, char *argv[])
{
    uint32_t id_in = 3;
    uint32_t id_out = 0;
    DplRef *r;

    DPL_ASSERT_OK (dpl_ref_new (&r));
    DPL_ASSERT_OK (dpl_ref_id_set (r, id_in));
    DPL_ASSERT_OK (dpl_ref_id_get (r, &id_out));
    DPL_ASSERT_EQ (id_in, id_out);

    DPL_ASSERT_OK (dpl_ref_free (r));

    return 0;
}
