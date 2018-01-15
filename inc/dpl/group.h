/* +
 */

#ifndef DAYPLAN_GROUP_H
# define DAYPLAN_GROUP_H

#include "dpl/list.h"


struct DplGroup_;

typedef struct DplGroup_ DplGroup;

int dpl_group_free (DplGroup *group);
/* Free the group object and its sucessors.
 *
 * DPL_OK
 *   Preconditions
 *     - group is allocated.
 */


int dpl_group_next (const DplGroup *group, DplGroup **next);
/* Return the successor group for a group.
 *
 * Preconditions
 *   - group is allocated and initialized.
 *
 * DPL_OK
 *   Preconditions
 *     - group has a successor.
 *   Postconditions
 *     - *next is assigned to the successor group of group.
 *
 * DPL_ITER_END
 *   Preconditions
 *     - group has no successor.
 */


int dpl_group_name_get (const DplGroup *group, const char **name);
/* Return the name of a group.
 *
 * DPL_OK
 *   Preconditions
 *     - group is allocated and initialized.
 *   Postconditions
 *     - The name of the group is assigned to *name.
 */


int dpl_group_entries_get (const DplGroup *group, DplIter **iter);
/* Return the list of entries belonging to the group.
 *
 * Preconditions
 *   - group is allocated and initialized.
 *
 * DPL_OK
 *   Preconditions
 *     - Memory can be allocated
 *   Postconditions
 *     - An iterator over the entries of the group is returned.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated
 */


/* Group entries by name.
 *
 * Every group contains entries that have the same name.
 * 
 * Name groups are ordered alphabetically.
 *
 * Parameters
 *   @iter An initialized iterator.
 *   @first This is set to point to the first group that is created.
 *
 * Returns
 *   DPL_OK on success, DPL_ERR_MEM if not enough memory can be allocated.
 */
int dpl_group_by_name (DplIter *iter, DplGroup **first);


int dpl_group_by_day (DplIter *iter, DplGroup **first);
/* Group entries by days.
 *
 * Every group contains entries that have their begin time on the same
 * day. The name of the group is the date of the day in ISO format.
 *
 * Preconditions
 *   - iter is an initialized iterator.
 *
 * DPL_OK
 *   Preconditions
 *     - Memory can be allocated
 *   Postconditions
 *     - *first contains a list of groups, in lexicographically ascending
 *     order. 
 *     - Every group contains the entries with begin time on the same day. 
 *     - All entries returned by iter are considered.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated
 *
 * DPL_ERR_SYS
 *   Preconditions
 *     - The begin time of an entry contained an invalid value, or the
 *     begin time value cannot be converted to a string.
 */


/* Group entries by category.
 *
 * Every group represents a category and contains entries of the category and
 * its subcategories. Subcategories in category names are introduced with a
 * slash (e. g. "job/project/a").
 *
 * Category groups are ordered alphabetically.
 *
 * Parameters
 *   @iter An initialized iterator.
 *   @first This is set to point to the first group that is created.
 *
 * Returns
 *   DPL_OK on success, DPL_ERR_MEM if not enough memory can be allocated.
 */
int dpl_group_by_category (DplIter *iter, DplGroup **first);


#endif /* DAYPLAN_GROUP_H */
