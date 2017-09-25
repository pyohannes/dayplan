/* +
 */

#include <stdlib.h>
#include "dpl/defs.h"
#include "dpl/tasklist.h"
#include "dpl/utils.h"


struct DplTaskListItem {
    DplTask *item;
    struct DplTaskListItem *next;
};


struct DplTaskListIter_
{
    void *data;
    int (*next_func)(struct DplTaskListIter_ *, DplTask **next);
    DplTaskListIter *source;
};


int dpl_tasklistiter_free (DplTaskListIter *iter)
{
    free (iter);

    return DPL_OK;
}


int dpl_tasklistiter_next (DplTaskListIter *iter, DplTask **next)
{
    if (iter->next_func) {
        return iter->next_func (iter, next);
    } else {
        struct DplTaskListItem *item = (struct DplTaskListItem *)iter->data;

        if (item) {
            *next = item->item;
            iter->data = (void *)item->next;
            return DPL_OK;
        } else {
            *next = 0;
            return DPL_ITER_END;
        }
    }
}


struct DplTaskList_
{
    uint32_t len;
    struct DplTaskListItem *first;
    struct DplTaskListItem *last;
};


int dpl_tasklist_new (DplTaskList **list)
{
    *list = malloc (sizeof (DplTaskList));
    if (!*list) {
        return DPL_ERR_MEM;
    }

    (*list)->len = 0;
    (*list)->first = 0;
    (*list)->last = 0;

    return DPL_OK;
}


int dpl_tasklist_free (DplTaskList *list, int freetasks)
{
    struct DplTaskListItem *next = list->first;
    struct DplTaskListItem *curr;

    while (next) {
        curr = next;
        next = next->next;
        if (freetasks) {
            dpl_task_free (curr->item);
        }
        free (curr);
    }

    free (list);

    return DPL_OK;
}


int dpl_tasklist_len (const DplTaskList *list, uint32_t *len)
{
    *len = list->len;

    return DPL_OK;
}


int dpl_tasklist_iter (const DplTaskList *list, DplTaskListIter **iter)
{
    *iter = malloc (sizeof (DplTaskListIter));

    if (!*iter) {
        return DPL_ERR_MEM;
    }

    (*iter)->data = (void *)list->first;
    (*iter)->next_func = 0;
    (*iter)->source = 0;

    return DPL_OK;
}

int dpl_tasklist_push (DplTaskList *list, const DplTask *task)
{
    struct DplTaskListItem *new = malloc (sizeof (struct DplTaskListItem));

    new->item = task;
    new->next = 0;

    if (!new) {
        return DPL_ERR_MEM;
    }

    if (list->last) {
        list->last->next = new;
        list->last = new;
    } else {
        list->last = list->first = new;
    }

    list->len += 1;

    return DPL_OK;
}


int dpl_tasklist_remove (DplTaskList *list, const DplTask *task)
{
    struct DplTaskListItem *curr;
    struct DplTaskListItem *prev;

    /* special case: task is first item in the list */
    if (list->first && list->first->item == task) {
        curr = list->first->next;
        free (list->first);
        list->first = curr;
        list->len -= 1;
        return DPL_OK;
    }

    curr = list->first;
   
    while (curr->next) {
        if (curr->next->item == task) {
            struct DplTaskListItem *newnext = curr->next->next;
            free (curr->next);
            curr->next = newnext;
            list->len -= 1;
            return DPL_OK;
        } 
        curr = curr->next;
    } 

    return DPL_ERR_NOTFOUND;
}


struct DplTaskListFilter_
{
    int (*next_func)(struct DplTaskListIter_ *, DplTask **next);
    int (*free_func)(struct DplTaskListFilter_ *);
    void *data;
};


int dpl_tasklist_filter_simple_free_ (struct DplTaskListFilter_ *filter)
{
    free (filter);

    return DPL_OK;
}


static int dpl_tasklist_filter_done_next_ (struct DplTaskListIter_ *iter, 
        DplTask **next)
{
    DplTask *task;
    int ret;
    int donevalue = (int)iter->data;

    while ((ret = dpl_tasklistiter_next (iter->source, &task)) == DPL_OK) {
        DplRef *ref;
        int done;
        DPL_FORWARD_ERROR (dpl_task_ref_get (task, &ref));
        if (!ref) {
            continue;
        }
        DPL_FORWARD_ERROR (dpl_ref_done_get (ref, &done));
        if (done == donevalue) {
            *next = task;
            return DPL_OK;
        }
    }

    return ret;
}


static int dpl_tasklist_filter_done_ (DplTaskListFilter **filter, int donevalue)
{
    *filter = malloc (sizeof (struct DplTaskListFilter_));

    if (!*filter) {
        return DPL_ERR_MEM;
    }

    (*filter)->data = donevalue;
    (*filter)->next_func = dpl_tasklist_filter_done_next_;
    (*filter)->free_func = dpl_tasklist_filter_simple_free_;

    return DPL_OK;
}


int dpl_tasklist_filter_done (DplTaskListFilter **filter)
{
    return dpl_tasklist_filter_done_ (filter, 1);
}


int dpl_tasklist_filter_undone (DplTaskListFilter **filter)
{
    return dpl_tasklist_filter_done_ (filter, 0);
}


static int dpl_tasklist_filter_tasks_next_ (struct DplTaskListIter_ *iter, 
        DplTask **next)
{
    DplTask *task;
    int ret;

    while ((ret = dpl_tasklistiter_next (iter->source, &task)) == DPL_OK) {
        DplTaskType type;
        DPL_FORWARD_ERROR (dpl_task_type_get (task, &type));
        if (type == task_type) {
            *next = task;
            return DPL_OK;
        }
    }

    return ret;
}


int dpl_tasklist_filter_tasks (DplTaskListFilter **filter)
{
    *filter = malloc (sizeof (struct DplTaskListFilter_));

    if (!*filter) {
        return DPL_ERR_MEM;
    }

    (*filter)->data = 0;
    (*filter)->next_func = dpl_tasklist_filter_tasks_next_;
    (*filter)->free_func = dpl_tasklist_filter_simple_free_;

    return DPL_OK;
}


static int dpl_tasklist_filter_refs_next_ (struct DplTaskListIter_ *iter, 
        DplTask **next)
{
    DplTask *task;
    int ret;

    while ((ret = dpl_tasklistiter_next (iter->source, &task)) == DPL_OK) {
        DplTaskType type;
        DPL_FORWARD_ERROR (dpl_task_type_get (task, &type));
        if (type == ref_type) {
            *next = task;
            return DPL_OK;
        }
    }

    return ret;
}


int dpl_tasklist_filter_refs (DplTaskListFilter **filter)
{
    *filter = malloc (sizeof (struct DplTaskListFilter_));

    if (!*filter) {
        return DPL_ERR_MEM;
    }

    (*filter)->data = 0;
    (*filter)->next_func = dpl_tasklist_filter_refs_next_;
    (*filter)->free_func = dpl_tasklist_filter_simple_free_;

    return DPL_OK;
}


struct DplTaskListFilter_Period_
{
    time_t begin;
    time_t end;
};


int dpl_tasklist_filter_period_next_ (struct DplTaskListIter_ *iter, 
        DplTask **next)
{
    struct DplTaskListFilter_Period_ *data = 
        (struct DplTaskListFilter_Period_ *)iter->data;
    DplTask *task;
    int ret;

    while ((ret = dpl_tasklistiter_next (iter->source, &task)) == DPL_OK) {
        time_t ibegin, iend;
        DPL_FORWARD_ERROR (dpl_task_begin_get (task, &ibegin));
        DPL_FORWARD_ERROR (dpl_task_end_get (task, &iend));
        if (data->begin <= ibegin && data->end >= iend) {
            *next = task;
            return DPL_OK;
        }
    }

    return ret;
}


int dpl_tasklist_filter_period_free_ (struct DplTaskListFilter_ *filter)
{
    free (filter->data);
    free (filter);

    return DPL_OK;
}


/* mention inclusive filter */
int dpl_tasklist_filter_period (time_t start, time_t end, 
        DplTaskListFilter **filter)
{
    struct DplTaskListFilter_Period_ *data;

    *filter = malloc (sizeof (struct DplTaskListFilter_));
    data = malloc (sizeof (struct DplTaskListFilter_Period_));

    if (!*filter || !data) {
        return DPL_ERR_MEM;
    }

    data->begin = start;
    data->end = end;
    (*filter)->data = (void *)data;
    (*filter)->next_func = dpl_tasklist_filter_period_next_;
    (*filter)->free_func = dpl_tasklist_filter_period_free_;

    return DPL_OK;
}


int dpl_tasklist_filter_today (DplTaskListFilter **filter)
{
    time_t now = time (0);
    time_t begin;
    struct tm tm;

    if (!localtime_r (&now, &tm)) {
        return DPL_ERR_SYS;
    }

    tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
    begin = mktime (&tm);

    return dpl_tasklist_filter_period (begin, begin + (60 * 60 * 24) - 1, 
            filter);
}


int dpl_tasklist_filter_free (DplTaskListFilter *filter)
{
    filter->free_func (filter);
}


int dpl_tasklist_filter (DplTaskListIter *in, DplTaskListFilter *filter, 
        DplTaskListIter **out)
{
    *out = malloc (sizeof (DplTaskListIter));

    if (!*out) {
        return DPL_ERR_MEM;
    }

    (*out)->next_func = filter->next_func;
    (*out)->data = filter->data;
    (*out)->source = in;

    return DPL_OK;
}
