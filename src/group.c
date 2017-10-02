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
    DplTaskList *tasks;
};


static int dpl_group_new (DplGroup **group)
{
    *group = malloc (sizeof (DplGroup));

    if (!*group) {
        return DPL_ERR_MEM;
    }

    (*group)->next = 0;
    (*group)->name = 0;
    (*group)->tasks = 0;

    return DPL_OK;
}


int dpl_group_free (DplGroup *group)
{
    free (group->name);
    free (group->tasks);
    if (group->next) {
        dpl_group_free (group->next);
    }
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


int dpl_group_tasks_get (const DplGroup *group, DplTaskListIter **iter)
{
    return dpl_tasklist_iter (group->tasks, iter);
}


static int _dpl_group_add_for_name (const char *name, DplTask *task, 
        DplGroup **first) 
{
    DplGroup *found = 0;

    if (!*first) {
        DPL_FORWARD_ERROR (dpl_group_new (first));
        DPL_STRDUP ((*first)->name, name);
        found = *first;
    } else {
        DplGroup *g = *first;
        DplGroup *prev = 0;

        while (!found) {
            int res = strcmp (name, g->name);
            if (res == 0) {
                found = g;
                break;
            } else if (res < 0) {
                DplGroup *newgroup;
                DPL_FORWARD_ERROR (dpl_group_new (&newgroup));
                newgroup->next = g;
                DPL_STRDUP (newgroup->name, name);
                if (prev) {
                    prev->next = newgroup;
                } else {
                    *first = newgroup;
                }
                found = newgroup;
                break;
            }

            if (g->next) {
                prev = g;
                g = g->next;
            } else {
                /* end of list */
                DplGroup *newgroup;
                DPL_FORWARD_ERROR (dpl_group_new (&newgroup));
                g->next = newgroup;
                DPL_STRDUP (newgroup->name, name);
                found = newgroup;
                break;
            }
        }
    }

    if (!found->tasks) {
        DPL_FORWARD_ERROR (dpl_tasklist_new (&found->tasks));
    }
    DPL_FORWARD_ERROR (dpl_tasklist_push (found->tasks, task));

    return DPL_OK;
}


int dpl_group_by_title (DplTaskListIter *iter, DplGroup **first)
{
    DplTask *task;
    const char *title;
    size_t titlepart_size = 1024;
    char *titlepart = malloc (titlepart_size + 1);

    *first = 0;

    while (dpl_tasklistiter_next (iter, &task) == DPL_OK) {
        int pos;

        DPL_FORWARD_ERROR (dpl_task_title_get (task, &title));
        pos = strlen (title);
        if (pos > titlepart_size) {
            titlepart_size = pos * 2;
            if (!(titlepart = malloc (titlepart_size))) {
                free (titlepart);
                return DPL_ERR_MEM;
            }
        }
        strcpy (titlepart, title);

        /* full word */
        DPL_FORWARD_ERROR (_dpl_group_add_for_name (title, task, first));

        while (pos) {
            if (titlepart[pos] == '/') {
                titlepart[pos] = '\0';
                DPL_FORWARD_ERROR (
                        _dpl_group_add_for_name (titlepart, task, first));
            }
            pos -= 1;
        }

    }

    free (titlepart);
    return DPL_OK;
}


int dpl_group_by_day (DplTaskListIter *iter, DplGroup **first)
{
    DplTask *task;
    time_t begin;
    struct tm tm_begin;
    char date[16] = "";

    *first = 0;

    while (dpl_tasklistiter_next (iter, &task) == DPL_OK) {
        DPL_FORWARD_ERROR (dpl_task_begin_get (task, &begin));
        localtime_r (&begin, &tm_begin);
        if (strftime (date, 16, "%Y-%m-%d", &tm_begin) < 10) {
            return DPL_ERR_SYS;
        }
        DPL_FORWARD_ERROR (_dpl_group_add_for_name (date, task, first));
    }

    return DPL_OK;
}
