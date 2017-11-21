/* +
 */

#include <stdlib.h>
#include <time.h>
#include "dpl/group.h"
#include "dpl/defs.h"
#include "dpl/utils.h"


struct DplGroup_
{
    DplGroup *next;
    char *name;
    DplList *entries;
};


static int dpl_group_new (DplGroup **group, const char *name)
/* Allocate and initialize new group object.
 *
 * DPL_OK
 *   Preconditions
 *     - Memory can be allocated.
 *   Postconditions
 *     - *group points to a newly allocated group.
 *     - *group contains an empty list of entries.
 *     - The name of *group is set to name.
 *
 * DPL_ERR
 *   Preconditions
 *     - Memory cannot be allocated.
 */
{
    *group = malloc (sizeof (DplGroup));

    if (!*group) {
        return DPL_ERR_MEM;
    }

    (*group)->next = 0;
    (*group)->name = 0;

    DPL_FORWARD_ERROR (dpl_list_new (&(*group)->entries));
    DPL_STRDUP ((*group)->name, name);

    return DPL_OK;
}


int dpl_group_free (DplGroup *group)
{
    if (group->name) {
        free (group->name);
    }
    if (group->next) {
        dpl_group_free (group->next);
    }
    dpl_list_free (group->entries, 0);
    free (group);

    return DPL_OK;
}


int dpl_group_next (const DplGroup *group, DplGroup **next)
{
    if (group->next) {
        *next = group->next;
        return DPL_OK;
    } else {
        return DPL_ITER_END;
    }
}


int dpl_group_name_get (const DplGroup *group, const char **name)
{
    *name = group->name;

    return DPL_OK;
}


int dpl_group_entries_get (const DplGroup *group, DplIter **iter)
{
    return dpl_list_iter (group->entries, iter);
}


static int _dpl_group_find (DplGroup *first, const char *name, 
        DplGroup **found)
/* Finds a group of the given name.
 *
 * Preconditions
 *   - first is allocated and initialized.
 *   - *first and its successor groups are sorted by name in ascending order.
 *   - name is not 0.
 *
 * DPL_OK
 *   Preconditions
 *     - A group with the given name was found.
 *   Postconditions
 *     - A pointer to the group is stored in *found.
 *
 * DPL_ERR_NOTFOUND
 *   Preconditions
 *     - A group with the given name was not found.
 */
{
    int ret;

    if ((ret = strcmp (name, first->name)) == 0) {
        *found = first;
        return DPL_OK;
    } else {
        if (ret < 0 || !first->next) {
            return DPL_ERR_NOTFOUND;
        }
        return _dpl_group_find (first->next, name, found);
    }
}


static int _dpl_group_add (DplGroup **first, const char *name, DplGroup **new)
/* Add a new group with the given name.
 *
 * Preconditions
 *   - name is not 0.
 *   - If *first is not 0, *first and its successor groups are sorted by name 
 *     in ascending order.
 *
 * DPL_OK
 *   Preconditions
 *     - Memory can be allocated.
 *   Postconditions
 *     - A new group with the given name is inserted in the orderd list
 *       *first.
 *     - A pointer to the new group is stored in *new.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated.
 */
{
    if (!*first || strcmp (name, (*first)->name) < 0) {
        DPL_FORWARD_ERROR (dpl_group_new (new, name));
        (*new)->next = *first;
        *first = *new;
    } else {
        DPL_FORWARD_ERROR (_dpl_group_add (&((*first)->next), name, new));
    }

    return DPL_OK;
}


static int _dpl_group_add_for_name (const char *name, const DplEntry *entry, 
        DplGroup **first) 
/* Add an entry into a named group.
 *
 * Preconditions
 *   - name is not 0.
 *
 * DPL_OK
 *   Preconditions
 *     - Memory can be allocated.
 *   Postconditions
 *     - If a group of the given name does not exist, it is created.
 *     - entry is added to the group with the given name.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated.
 */
{
    DplGroup *found = 0;

    if (!*first || (_dpl_group_find (*first, name, &found) != DPL_OK)) {
        DPL_FORWARD_ERROR (_dpl_group_add (first, name, &found));
    }

    DPL_FORWARD_ERROR (dpl_list_push (found->entries, entry));

    return DPL_OK;
}


int dpl_group_by_title (DplIter *iter, DplGroup **first)
{
    const DplEntry *entry;
    const char *title;
    int titlepart_size = 1024;
    char *titlepart = malloc (titlepart_size + 1);

    *first = 0;

    while (dpl_iter_next (iter, &entry) == DPL_OK) {
        int pos;

        dpl_entry_name_get (entry, &title);
        DPL_STRCPY (titlepart, title, titlepart_size);

        /* full word */
        DPL_FORWARD_ERROR (_dpl_group_add_for_name (title, entry, first));

        /* add partial words (categories) */
        pos = strlen (titlepart);
        while (pos) {
            if (titlepart[pos] == '/') {
                titlepart[pos] = '\0';
                DPL_FORWARD_ERROR (
                        _dpl_group_add_for_name (titlepart, entry, first));
            }
            pos -= 1;
        }

    }

    free (titlepart);
    return DPL_OK;
}


int dpl_group_by_day (DplIter *iter, DplGroup **first)
{
    const DplEntry *entry;
    time_t begin;
    struct tm tm_begin;
    char date[16] = "";

    *first = 0;

    while (dpl_iter_next (iter, &entry) == DPL_OK) {
        dpl_entry_begin_get (entry, &begin);
        localtime_r (&begin, &tm_begin);
        if (strftime (date, 16, "%Y-%m-%d", &tm_begin) < 10) {
            return DPL_ERR_SYS;
        }
        DPL_FORWARD_ERROR (_dpl_group_add_for_name (date, entry, first));
    }

    return DPL_OK;
}
