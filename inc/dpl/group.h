/* +
 */

#ifndef DAYPLAN_GROUP_H
# define DAYPLAN_GROUP_H

#include "dpl/tasklist.h"


struct DplGroup_;

typedef struct DplGroup_ DplGroup;

int dpl_group_free (DplGroup *group);

int dpl_group_next (const DplGroup *group, DplGroup **next);
int dpl_group_name_get (const DplGroup *group, const char **name);
int dpl_group_tasks_get (const DplGroup *group, DplTaskListIter **iter);

int dpl_group_by_title (DplTaskListIter *iter, DplGroup **first);
int dpl_group_by_day (DplTaskListIter *iter, DplGroup **first);


#endif /* DAYPLAN_GROUP_H */