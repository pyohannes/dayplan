/* +
 */

#include "dpl/acc.h"
#include "dpl/defs.h"
#include "dpl/utils.h"

int dpl_acc_durance (DplTaskListIter *iter, time_t *dur)
{
    DplTask *task;
    *dur = 0;
    time_t begin, end;

    while (dpl_tasklistiter_next (iter, &task) == DPL_OK) {
        DPL_FORWARD_ERROR (dpl_task_begin_get (task, &begin));
        DPL_FORWARD_ERROR (dpl_task_end_get (task, &end));
        *dur += (end - begin);
    }

    return DPL_OK;
}
