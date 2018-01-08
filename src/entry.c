#include <stdlib.h>
#include <string.h>
#include "dpl/entry.h"
#include "dpl/utils.h"
#include "dpl/defs.h"


#define ENSURE_ENTRY_TYPE(DPL_entry, DPL_type) \
    DPL_FORWARD_ERROR (DPL_entry->type == DPL_type ? DPL_OK : DPL_ERR_TYPE)


struct DplWork_ 
{
    time_t end;
    const DplEntry *task;
};


struct DplTask_
{
    uint32_t id;
    int done;
};


struct DplEntry_
{
    char *name;
    char *desc;
    time_t begin;
    DplEntryType type;
    union {
        struct DplTask_ task;
        struct DplWork_ work;
    } ext;
};


int dpl_entry_new (DplEntry **entry, DplEntryType type)
{
    *entry = malloc (sizeof (DplEntry));
    if (!*entry) {
        return DPL_ERR_MEM;
    }

    (*entry)->name = 0;
    (*entry)->desc = 0;
    (*entry)->begin = 0;
    (*entry)->type = type;

    if (type == ENTRY_WORK) {
        (*entry)->ext.work.end = 0;
        (*entry)->ext.work.task = 0;
    } else {
        (*entry)->ext.task.id = 0;
        (*entry)->ext.task.done = 0;
    }

    return DPL_OK;
}


int dpl_entry_free (DplEntry *entry)
{
    if (entry->name) {
        free (entry->name);
    }
    if (entry->desc) {
        free (entry->desc);
    }

    free (entry);

    return DPL_OK;
}



int dpl_entry_name_get (const DplEntry *entry, const char **name)
{
    *name = entry->name;

    return DPL_OK;
}


int dpl_entry_desc_get (const DplEntry *entry, const char **desc)
{
    *desc = entry->desc;

    return DPL_OK;
}


int dpl_entry_begin_get (const DplEntry *entry, time_t *begin)
{
    *begin = entry->begin;

    return DPL_OK;
}


int dpl_entry_work_end_get (const DplEntry *entry, time_t *end)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_WORK);

    *end = entry->ext.work.end;

    return DPL_OK;
}


int dpl_entry_work_task_get (const DplEntry *entry, const DplEntry **task)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_WORK);

    *task= entry->ext.work.task;

    return DPL_OK;
}


int dpl_entry_type_get (const DplEntry *entry, DplEntryType *type)
{
    *type = entry->type;

    return DPL_OK;
}


int dpl_entry_name_set (DplEntry *entry, const char *name)
{
    DPL_STRDUP (entry->name, name);

    return DPL_OK;
}


int dpl_entry_desc_set (DplEntry *entry, const char *desc)
{
    DPL_STRDUP (entry->desc, desc);

    return DPL_OK;
}


int dpl_entry_begin_set (DplEntry *entry, time_t begin)
{
    entry->begin = begin;

    return DPL_OK;
}


int dpl_entry_work_end_set (DplEntry *entry, time_t end)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_WORK);

    entry->ext.work.end = end;

    return DPL_OK;
}


int dpl_entry_work_task_set (DplEntry *entry, const DplEntry *task)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_WORK);

    entry->ext.work.task = task;

    return DPL_OK;
}


int dpl_entry_type_set (DplEntry *entry, DplEntryType type)
{
    entry->type = type;

    return DPL_OK;
}


int dpl_entry_task_id_get (const DplEntry *entry, uint32_t *id)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_TASK);

    *id = entry->ext.task.id;

    return DPL_OK;
}


int dpl_entry_task_done_get (const DplEntry *entry, int *done)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_TASK);

    *done = entry->ext.task.done;

    return DPL_OK;
}



int dpl_entry_task_id_set (DplEntry *entry, uint32_t id)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_TASK);

    entry->ext.task.id = id;

    return DPL_OK;
}

int dpl_entry_task_done_set (DplEntry *entry, int done)
{
    ENSURE_ENTRY_TYPE(entry, ENTRY_TASK);

    entry->ext.task.done = done;

    return DPL_OK;
}
