/* +
 */

#ifndef DAYPLAN_ACC_H
# define DAYPLAN_ACC_H

#include "dpl/list.h"


int dpl_acc_durance (DplIter *iter, time_t *dur);
/* Accumulate the durance of entries in iter.
 *
 * Preconditions
 *   - iter is initialized.
 *   - dur is not 0.
 *
 * DPL_OK
 *   Preconditions
 *     - All entries in iter are of type ENTRY_WORK
 *   Postconditions
 *     - *dur contains the sum of the differences of end time and start
 *     time of each entry in iter.
 *
 * DPL_ERR_TYPE
 *   Preconditions
 *     - Not all entries in iter are of type ENTRY_WORK
 */


#endif /* DAYPLAN_ACC_H */
