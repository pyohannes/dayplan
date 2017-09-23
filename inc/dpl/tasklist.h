/* +
 */

#ifndef DAYPLAN_TASKLIST_H
# define DAYPLAN_TASKLIST_H

# include <stdint.h>
# include "dpl/task.h"



struct DplTaskListIter_;
typedef struct DplTaskListIter_ DplTaskListIter;


int dpl_tasklistiter_free (DplTaskListIter *iter);
int dpl_tasklistiter_next (DplTaskListIter *iter, DplTask **next);


struct DplTaskList_;
typedef struct DplTaskList_ DplTaskList;


int dpl_tasklist_new (DplTaskList **list);
int dpl_tasklist_free (DplTaskList *list, int freetasks);

int dpl_tasklist_len (const DplTaskList *list, uint32_t *len);
int dpl_tasklist_iter (const DplTaskList *list, DplTaskListIter **iter);

int dpl_tasklist_push (DplTaskList *list, const DplTask *task);
int dpl_tasklist_remove (DplTaskList *list, const DplTask *task);

struct DplTaskListFilter_;
typedef struct DplTaskListFilter_ DplTaskListFilter;

int dpl_tasklist_filter_period (time_t start, time_t end, 
        DplTaskListFilter **filter);
int dpl_tasklist_filter_today (DplTaskListFilter **filter);
int dpl_tasklist_filter_tasks (DplTaskListFilter **filter);
int dpl_tasklist_filter_refs (DplTaskListFilter **filter);
int dpl_tasklist_filter_done (DplTaskListFilter **filter);
int dpl_tasklist_filter_undone (DplTaskListFilter **filter);

int dpl_tasklist_filter_free (DplTaskListFilter *filter);

int dpl_tasklist_filter (DplTaskListIter *in, DplTaskListFilter *filter, 
        DplTaskListIter **out);


#endif /* DAYPLAN_TASKLIST_H */
