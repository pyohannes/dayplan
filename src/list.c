/* +
 */

#include <stdlib.h>
#include "dpl/defs.h"
#include "dpl/list.h"
#include "dpl/utils.h"


struct DplListItem {
    const DplEntry *item;
    struct DplListItem *next;
};


struct DplIter_
{
    void *data;
    int free_data;
    int (*next_func)(struct DplIter_ *, const DplEntry **next);
    DplIter *source;
};


struct DplList_
{
    uint32_t len;
    struct DplListItem *first;
    struct DplListItem *last;
};


int dpl_iter_free (DplIter *iter)
{
    if (iter->free_data) {
        free (iter->data);
    }

    free (iter);

    return DPL_OK;
}


int dpl_iter_next (DplIter *iter, const DplEntry **next)
{
    return iter->next_func (iter, next);
}


int dpl_iter_list (DplIter *iter, DplList **list)
{
    const DplEntry *entry;

    DPL_FORWARD_ERROR (dpl_list_new (list));

    while (dpl_iter_next (iter, &entry) == DPL_OK) {
        DPL_FORWARD_ERROR (dpl_list_push (*list, entry));
    }

    return DPL_OK;
}


int dpl_list_new (DplList **list)
{
    *list = malloc (sizeof (DplList));
    if (!*list) {
        return DPL_ERR_MEM;
    }

    (*list)->len = 0;
    (*list)->first = 0;
    (*list)->last = 0;

    return DPL_OK;
}


int dpl_list_free (DplList *list, int freeentries)
{
    struct DplListItem *next = list->first;
    struct DplListItem *curr;

    while (next) {
        curr = next;
        next = next->next;
        if (freeentries) {
            dpl_entry_free ((DplEntry *)curr->item);
        }
        free (curr);
    }

    free (list);

    return DPL_OK;
}


int dpl_list_len (const DplList *list, uint32_t *len)
{
    *len = list->len;

    return DPL_OK;
}


static int dpl_list_iter_next_ (DplIter *iter, const DplEntry **next)
{
    struct DplListItem *item = (struct DplListItem *)iter->data;

    if (item) {
        *next = item->item;
        iter->data = (void *)item->next;
        return DPL_OK;
    } else {
        *next = 0;
        return DPL_ITER_END;
    }
}


static int dpl_iter_new (DplIter **iter)
{
    *iter= malloc (sizeof (DplIter));

    if (!*iter) {
        return DPL_ERR_MEM;
    }

    (*iter)->data = 0;
    (*iter)->next_func = 0;
    (*iter)->free_data = 0;
    (*iter)->source = 0;

    return DPL_OK;
}


int dpl_list_iter (const DplList *list, DplIter **iter)
{
    DPL_FORWARD_ERROR (dpl_iter_new (iter));

    (*iter)->data = (void *)list->first;
    (*iter)->next_func = dpl_list_iter_next_;

    return DPL_OK;
}


int dpl_list_push (DplList *list, const DplEntry *entry)
{
    struct DplListItem *new = malloc (sizeof (struct DplListItem));

    if (!new) {
        return DPL_ERR_MEM;
    }

    new->item = entry;
    new->next = 0;

    if (list->last) {
        list->last->next = new;
        list->last = new;
    } else {
        list->last = list->first = new;
    }

    list->len += 1;

    return DPL_OK;
}


int dpl_list_remove (DplList *list, const DplEntry *entry)
{
    struct DplListItem *curr;
    struct DplListItem *prev;

    /* special case: entry is first item in the list */
    if (list->first && list->first->item == entry) {
        curr = list->first->next;
        free (list->first);
        list->first = curr;
        list->len -= 1;
        return DPL_OK;
    }

    curr = list->first;
   
    while (curr->next) {
        if (curr->next->item == entry) {
            struct DplListItem *newnext = curr->next->next;
            free (curr->next);
            curr->next = newnext;
            list->len -= 1;
            return DPL_OK;
        } 
        curr = curr->next;
    } 

    return DPL_ERR_NOTFOUND;
}


static int dpl_filter_done_next_ (struct DplIter_ *iter, const DplEntry **next)
{
    const DplEntry *entry;
    int ret;
    int donevalue = (int)(long)iter->data;

    while ((ret = dpl_iter_next (iter->source, &entry)) == DPL_OK) {
        const DplEntry *ref;
        int done;
        DplEntryType type;

        dpl_entry_type_get (entry, &type);

        if (type == ENTRY_WORK) {
            dpl_entry_work_task_get (entry, &ref);
        } else {
            ref = entry;
        }

        if (!ref) {
            continue;
        }

        DPL_FORWARD_ERROR (dpl_entry_task_done_get (ref, &done));
        if (done == donevalue) {
            *next = entry;
            return DPL_OK;
        }
    }

    return ret;
}


int dpl_filter_done (DplIter *in, int done, DplIter **out)
{
    DPL_FORWARD_ERROR (dpl_iter_new (out));

    (*out)->data = (void *)(long)done;
    (*out)->next_func = dpl_filter_done_next_;
    (*out)->source = in;

    return DPL_OK;
}


static int dpl_filter_type_next_ (struct DplIter_ *iter, const DplEntry **next)
{
    const DplEntry *entry;
    int ret;

    while ((ret = dpl_iter_next (iter->source, &entry)) == DPL_OK) {
        DplEntryType type;
        DPL_FORWARD_ERROR (dpl_entry_type_get (entry, &type));
        if (type == (DplEntryType)(iter->data)) {
            *next = entry;
            return DPL_OK;
        }
    }

    return ret;
}


int dpl_filter_type (DplIter *in, DplEntryType type, DplIter **out)
{
    DPL_FORWARD_ERROR (dpl_iter_new (out));

    (*out)->data = (void *)type;
    (*out)->next_func = dpl_filter_type_next_;
    (*out)->source = in;

    return DPL_OK;
}


struct DplFilter_Period_
{
    time_t begin;
    time_t end;
};


int dpl_filter_period_next_ (struct DplIter_ *iter, const DplEntry **next)
{
    struct DplFilter_Period_ *data;
    const DplEntry *entry;
    int ret;

    data = (struct DplFilter_Period_ *)iter->data;

    while ((ret = dpl_iter_next (iter->source, &entry)) == DPL_OK) {
        time_t ibegin, iend;

        DPL_FORWARD_ERROR (dpl_entry_begin_get (entry, &ibegin));
        if (dpl_entry_work_end_get (entry, &iend) == DPL_ERR_TYPE) {
            iend = ibegin;
        }
        if (data->begin <= ibegin && data->end >= iend) {
            *next = entry;
            return DPL_OK;
        }
    }

    return ret;
}


int dpl_filter_period (DplIter *in, time_t start, time_t end, DplIter **out)
{
    struct DplFilter_Period_ *data;
   
    data = malloc (sizeof (struct DplFilter_Period_));

    if (data) {
        return DPL_ERR_MEM;
    }

    data->begin = start;
    data->end = end;

    DPL_FORWARD_ERROR (dpl_iter_new (out));
    (*out)->data = (void *)data;
    (*out)->next_func = dpl_filter_period_next_;
    (*out)->free_data = 1;
    (*out)->source = in;

    return DPL_OK;
}
