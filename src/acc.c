/* +
 */

#include "dpl/acc.h"
#include "dpl/defs.h"
#include "dpl/utils.h"


int dpl_acc_durance (DplIter *iter, time_t *dur)
{
    const DplEntry *task;
    *dur = 0;
    time_t begin, end;

    while (dpl_iter_next (iter, &task) == DPL_OK) {
        dpl_entry_begin_get (task, &begin);
        DPL_FORWARD_ERROR (dpl_entry_work_end_get (task, &end));
        *dur += (end - begin);
    }

    return DPL_OK;
}
