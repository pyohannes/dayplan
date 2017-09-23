/* +
 */

#ifndef DAYPLAN_TASK_H
# define DAYPLAN_TASK_H

# include <time.h>
# include "dpl/ref.h"


struct DplTask_;

typedef struct DplTask_ DplTask;

typedef enum {
    task_type,
    ref_type
} DplTaskType;

int dpl_task_new (DplTask **task);
int dpl_task_free (DplTask *task);

int dpl_task_title_get (const DplTask *task, const char **title);
int dpl_task_begin_get (const DplTask *task, time_t *begin);
int dpl_task_end_get (const DplTask *task, time_t *end);
int dpl_task_ref_get (const DplTask *task, const DplRef **ref);
int dpl_task_desc_get (const DplTask *task, const char **desc);
int dpl_task_type_get (const DplTask *task, DplTaskType *type);

int dpl_task_title_set (DplTask *task, const char *title);
int dpl_task_begin_set (DplTask *task, time_t begin);
int dpl_task_end_set (DplTask *task, time_t end);
int dpl_task_ref_set (DplTask *task, const DplRef *ref);
int dpl_task_desc_set (DplTask *task, const char *desc);
int dpl_task_type_set (DplTask *task, DplTaskType type);


#endif /* DAYPLAN_TASK_H */
