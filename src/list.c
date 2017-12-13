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
    void (*free_data)(void *);
    int (*next_func)(struct DplIter_ *, const DplEntry **next);
    DplIter *source;
};


struct DplList_
{
    uint32_t len;
    struct DplListItem *first;
    struct DplListItem *last;
};


static int dpl_list_contains (const DplList *l, const DplEntry *entry, 
        int *contains)
/* Test if a list contains an entry object. It is not tested for equality, but 
 * for identity.
 *
 * Preconditions
 *   - list is allocated
 *
 * DPL_OK
 *   Preconditions
 *     - The list contains entry.
 *   Postconditions
 *     - *contains is set to 1
 *
 * DPL_OK
 *   Preconditions
 *     - The list does not contain entry.
 *   Postconditions
 *     - *contains is set to 0
 */
{
    struct DplListItem *pos = l->first;

    *contains = 0;

    while (pos) {
        if (pos->item == entry) {
            *contains = 1;
            break;
        }
        pos = pos->next;
    }

    return DPL_OK;
}


int dpl_iter_free (DplIter *iter)
{
    if (iter->free_data) {
        iter->free_data (iter->data);
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

    /* special case 1: entry is first item in the list */
    if (list->first && list->first->item == entry) {
        curr = list->first->next;

        if (list->first == list->last) {
            list->last = curr;
        }

        free (list->first);
        list->first = curr;
        list->len -= 1;
        return DPL_OK;
    }

    curr = list->first;
   
    while (curr->next) {
        if (curr->next->item == entry) {
            struct DplListItem *newnext = curr->next->next;

            if (curr->next == list->last) {
                list->last = curr;
            }

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

    if (!data) {
        return DPL_ERR_MEM;
    }

    data->begin = start;
    data->end = end;

    DPL_FORWARD_ERROR (dpl_iter_new (out));
    (*out)->data = (void *)data;
    (*out)->next_func = dpl_filter_period_next_;
    (*out)->free_data = free;
    (*out)->source = in;

    return DPL_OK;
}


int dpl_filter_task_for_work_next_ (struct DplIter_ *iter, 
        const DplEntry **next)
{
    const DplEntry *entry;
    int ret;

    while ((ret = dpl_iter_next (iter->source, &entry)) == DPL_OK) {
        DPL_FORWARD_ERROR (dpl_entry_work_task_get (entry, next));

        if (*next) {
            break;
        }
    }

    return ret;
}


int dpl_filter_task_for_work (DplIter *in, DplIter **out)
{
    DPL_FORWARD_ERROR (dpl_iter_new (out));
    (*out)->next_func = dpl_filter_task_for_work_next_;
    (*out)->free_data = 0;
    (*out)->source = in;

    return DPL_OK;
}


int dpl_filter_unique_next_ (struct DplIter_ *iter, const DplEntry **next)
{
    DplList *returned = (DplList *)iter->data;
    int ret, contains;

    while ((ret = dpl_iter_next (iter->source, next)) == DPL_OK) {

        DPL_FORWARD_ERROR (dpl_list_contains (returned, *next, &contains));

        if (contains) {
            continue;
        }

        DPL_FORWARD_ERROR (dpl_list_push (returned, *next));

        return DPL_OK;

    }

    return ret;
}


static void dpl_filter_unique_free (void *list)
{
    dpl_list_free ((DplList *)list, 0);
}


int dpl_filter_unique (DplIter *in, DplIter **out)
{
    DplList *returned;
    int ret;

    DPL_FORWARD_ERROR (dpl_list_new (&returned));

    if ((ret = dpl_iter_new (out)) != DPL_OK) {
        dpl_list_free (returned, 0);
        return ret;
    }

    (*out)->data = (void *)returned;
    (*out)->next_func = dpl_filter_unique_next_;
    (*out)->free_data = dpl_filter_unique_free;
    (*out)->source = in;

    return DPL_OK;
}


#define DPL_LISTITEMS_JOIN(DPL_items1, DPL_item2) { \
    struct DplListItem *pos = DPL_items1; \
    while (pos->next) { \
        pos = pos->next; \
    } \
    pos->next = DPL_item2; \
}


static int dpl_list_sort_quick_divide (struct DplListItem *input, 
        struct DplListItem **part1, struct DplListItem **part2)
/* Divide a list of entries by the smaller of the first two items.
 *
 * DPL_OK
 *   Preconditions
 *     - input is allocated.
 *     - input contains at least two items.
 *   Preconditions
 *     - The smaller of the first two items of input is the first item of part1.
 *     - All items smaller as the first item of part1 are stored in part1.
 *     - All items greater equal than the first item of part1 are stored
 *       in part2 (except the first item of part1).
 */
{
    struct DplListItem *pos1, *pos2;
    time_t begin1, begin2;

    /* ensure that the first item in part1 <= first item in part2 */
    *part1 = input;
    *part2 = input->next;
    dpl_entry_begin_get ((*part1)->item, &begin1);
    dpl_entry_begin_get ((*part2)->item, &begin2);

    if (begin1 > begin2) {
        (*part1) = input->next;
        (*part2) = input;
        begin1 = begin2;
    }

    input = input->next->next;
    pos1 = (*part1);
    pos2 = (*part2);

    /* ensure that items in part1 <= items in part2 */
    while (input) {
        dpl_entry_begin_get (input->item, &begin2);

        if (begin2 < begin1) {
            pos1->next = input;
            pos1 = pos1->next;
        } else {
            pos2->next = input;
            pos2 = pos2->next;
        }

        input = input->next;
    }

    /* ensure list end markers */
    pos1->next = 0;
    pos2->next = 0;

    return DPL_OK;
}


static int dpl_list_sort_quick (struct DplListItem **list)
/* Sort the entries in *list via a stable quicksort.
 *
 * DPL_OK
 *   Preconditions
 *     - *list is allocated.
 *   Postconditions
 *     - The original list is overwritten.
 *     - DplEntries in *list are sorted by ascending begin time.
 *     - The sort is stable.
 */
{
    struct DplListItem *part1, *part2;

    if (!*list || !(*list)->next) {
        return DPL_OK;
    }

    /* divide */
    dpl_list_sort_quick_divide (*list, &part1, &part2);

    /* recurse */
    dpl_list_sort_quick (&part1);
    dpl_list_sort_quick (&part2);

    /* merge */
    DPL_LISTITEMS_JOIN(part1, part2);

    *list = part1;

    return DPL_OK;
}


int dpl_list_sort (DplList **list)
{
    struct DplListItem *pos;

    dpl_list_sort_quick (&(*list)->first);

    /* reset last list item */
    pos = (*list)->first;

    while (pos && pos->next) {
        pos = pos->next;
    }

    (*list)->last = pos;

    return DPL_OK;
}
