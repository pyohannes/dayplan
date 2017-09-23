/* +
 */

#ifndef DAYPLAN_REF_H
# define DAYPLAN_REF_H

# include <stdint.h>


struct DplRef_;

typedef struct DplRef_ DplRef;


int dpl_ref_new (DplRef **ref);
int dpl_ref_free (DplRef *ref);


int dpl_ref_id_get (const DplRef *ref, uint32_t *id);
int dpl_ref_title_get (const DplRef *ref, const char **title);
int dpl_ref_desc_get (const DplRef *ref, const char **desc);
int dpl_ref_done_get (const DplRef *ref, int *done);

int dpl_ref_id_set (DplRef *ref, uint32_t id);
int dpl_ref_title_set (DplRef *ref, const char *title);
int dpl_ref_desc_set (DplRef *ref, const char *desc);
int dpl_ref_done_set (DplRef *ref, int done);


#endif /* DAYPLAN_REF_H */

