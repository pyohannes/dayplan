#include <stdlib.h>
#include <string.h>
#include "dpl/task.h"
#include "dpl/utils.h"
#include "dpl/defs.h"


struct DplTask_ 
{
    char *title;
    char *desc;
    time_t begin;
    time_t end;
    DplTaskType type;
    const DplRef *ref;
};


int dpl_task_new (DplTask **task)
{
    *task = malloc (sizeof (DplTask));
    if (!*task) {
        return DPL_ERR_MEM;
    }

    (*task)->title = 0;
    (*task)->desc= 0;
    (*task)->begin = (*task)->end = 0;
    (*task)->ref = 0;
    (*task)->type = task_type;

    return DPL_OK;
}


int dpl_task_free (DplTask *task)
{
    if (task->title) {
        free (task->title);
    }
    if (task->desc) {
        free (task->desc);
    }
    if (task->ref && (task->begin == 0) && (task->end == 0)) {
        free (task->ref);
    }

    free (task);
    return DPL_OK;
}



int dpl_task_title_get (const DplTask *task, const char **title)
{
    if (task->ref) {
        return dpl_ref_title_get (task->ref, title);
    } else {
        *title = task->title;

        return DPL_OK;
    }
}


int dpl_task_desc_get (const DplTask *task, const char **desc)
{
    *desc = task->desc;

    return DPL_OK;
}


int dpl_task_begin_get (const DplTask *task, time_t *begin)
{
    *begin = task->begin;

    return DPL_OK;
}


int dpl_task_end_get (const DplTask *task, time_t *end)
{
    *end = task->end;

    return DPL_OK;
}


int dpl_task_ref_get (const DplTask *task, const DplRef **ref)
{
    *ref = task->ref;

    return DPL_OK;
}

int dpl_task_type_get (const DplTask *task, DplTaskType *type)
{
    *type = task->type;

    return DPL_OK;
}

int dpl_task_title_set (DplTask *task, const char *title)
{
    DPL_STRDUP (task->title, title);

    return DPL_OK;
}


int dpl_task_desc_set (DplTask *task, const char *desc)
{
    DPL_STRDUP (task->desc, desc);

    return DPL_OK;
}


int dpl_task_begin_set (DplTask *task, time_t begin)
{
    task->begin = begin;

    return DPL_OK;
}


int dpl_task_end_set (DplTask *task, time_t end)
{
    task->end = end;

    return DPL_OK;
}


int dpl_task_ref_set (DplTask *task, const DplRef *ref)
{
    task->ref = ref;

    return DPL_OK;
}


int dpl_task_type_set (DplTask *task, DplTaskType type)
{
    task->type = type;

    return DPL_OK;
}
