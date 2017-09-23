#include <stdlib.h> 
#include "dpl/ref.h"
#include "dpl/utils.h"
#include "dpl/defs.h"


struct DplRef_
{
    uint32_t id;
    char *title;
    char *desc;
    int done;
};


int dpl_ref_new (DplRef **ref)
{
    *ref = malloc (sizeof (DplRef));

    if (!*ref) {
        return DPL_ERR_MEM;
    }

    (*ref)->id= 0;
    (*ref)->title = 0;
    (*ref)->desc= 0;
    (*ref)->done = 0;

    return DPL_OK;
}


int dpl_ref_free (DplRef *ref)
{
    free (ref);

    return DPL_OK;
}


int dpl_ref_id_get (const DplRef *ref, uint32_t *id)
{
    *id = ref->id;

    return DPL_OK;
}


int dpl_ref_title_get (const DplRef *ref, const char **title)
{
    *title = ref->title;

    return DPL_OK;
}


int dpl_ref_desc_get (const DplRef *ref, const char **desc)
{
    *desc = ref->desc;

    return DPL_OK;
}


int dpl_ref_done_get (const DplRef *ref, int *done)
{
    *done = ref->done;

    return DPL_OK;
}



int dpl_ref_id_set (DplRef *ref, uint32_t id)
{
    ref->id = id;

    return DPL_OK;
}


int dpl_ref_title_set (DplRef *ref, const char *title)
{
    DPL_STRDUP (ref->title, title);

    return DPL_OK;
}


int dpl_ref_desc_set (DplRef *ref, const char *desc)
{
    DPL_STRDUP (ref->desc, desc);

    return DPL_OK;
}


int dpl_ref_done_set (DplRef *ref, int done)
{
    ref->done = done;

    return DPL_OK;
}
