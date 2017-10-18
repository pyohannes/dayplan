/* +
 */

#ifndef DAYPLAN_LIST_H
# define DAYPLAN_LIST_H

# include <stdint.h>
# include "dpl/entry.h"


struct DplIter_;
typedef struct DplIter_ DplIter;

struct DplList_;
typedef struct DplList_ DplList;

int dpl_list_new (DplList **list);
/* Allocate and initialize a new list.
 *
 * DPL_OK
 *   Preconditions
 *     - Memory can be allocated
 *   Postconditions
 *     - *list points to a newly allocated and initialized list
 *     - The list is empty
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory can not be allocated
 */

int dpl_list_len (const DplList *list, uint32_t *len);
/* Return the length of a list.
 *
 * DPL_OK
 *   Precondition
 *     - list is allocated
 *   Postcondition
 *      - The number of entries in the list is assigned to *len
 */

int dpl_list_push (DplList *list, const DplEntry *entry);
/* Append an entry to a list.
 *
 * DPL_OK
 *   Preconditions
 *     - list is allocated
 *     - Memory can be allocated
 *   Postconditions
 *     - entry is appended to the list
 *     - list does not take ownership of entry
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory can be allocated
 */

int dpl_list_remove (DplList *list, const DplEntry *entry);
/* Remove an entry from a list.
 *
 * DPL_OK
 *   Preconditions
 *     - list is allocated
 *     - entry is in list
 *   Postconditions
 *     - The first occurence of entry is removed from list
 *     - entry is not freed
 *
 * DPL_ERR_NOT_FOUND
 *   Preconditions
 *     - entry is not in list
 */

int dpl_list_iter (const DplList *list, DplIter **iter);
/* Initialize an iterator that iterates over a list.
 *
 * DPL_OK
 *   Precondition
 *     - iter is allocated
 *     - Memory can be allocated
 *   Postcondition
 *      - An iterator over members of list is initialized and assigned to *iter
 *
 * DPL_ERR_MEM
 *   Precondition
 *     - iter is allocated
 *     - Memory can not be allocated
 */

int dpl_iter_free (DplIter *iter);
/* Free an iterator object.
 *
 * DPL_OK
 *   Precondition
 *     - iter is allocated
 *   Postcondition
 *      - Memory allocated to iter is freed.
 */

int dpl_iter_next (DplIter *iter, const DplEntry **next);
/* Return the next entry object of the iterator.
 *
 * DPL_OK
 *   Precondition
 *     - iter points to an iterator
 *     - The iterator is not exhausted
 *   Postcondition
 *     - next is set to point to the next entry object of iterator
 *     - iter is advanced by one element
 *
 * DPL_ITER_END
 *   Precondition
 *     - iter points to an iterator
 *     - iter is exhausted
 */

int dpl_iter_list (DplIter *iter, DplList **list);
/* Convert an iterator into a list.
 *
 * DPL_OK
 *   Preconditions
 *     - iter points to an iterator
 *     - Memory can be allocated
 *   Postcondition
 *     - *list points to a newly allocated list
 *     - all elements the iterator yields are appended to *list
 *     - iter is exhausted
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - iter points to an iterator
 *     - Memory cannot be allocated
 */

int dpl_list_free (DplList *list, int freeentries);
/* Free a list.
 *
 * DPL_OK
 *   Precondition
 *     - list is allocated
 *     - freeentries is 0
 *   Postcondition
 *      - Memory allocated to list is freed.
 *
 * DPL_OK
 *   Precondition
 *     - list is allocated
 *     - freeentries is not 0
 *   Postcondition
 *      - Memory allocated to list is freed.
 *      - dpl_entry_free is called on all entries in the list
 */

int dpl_filter_period (DplIter *in, time_t start, time_t end, DplIter **out);
/* Return an iterator that only yields elements in a time period.
 *
 * DPL_OK
 *   Precondition
 *     - in is allocated
 *     - Memory is available
 *   Postcondition
 *      - *out is assigned to an iterator that returns the elements of the 
 *      iterator in 
 *      - *out yields entries of type ENTRY_WORK for which e->begin >= start
 *      and e->end <= end
 *      - *out yields entries of type ENTRY_TASK for which 
 *      start <= e->begin <= end
 *
 * DPL_ERR_MEM
 *   Precondition
 *     - iter is allocated
 *     - Memory is not available
 */

int dpl_filter_type (DplIter *in, DplEntryType type, DplIter **out);
/* Return an iterator that only yields elements of a given type.
 *
 * DPL_OK
 *   Precondition
 *     - in is allocated
 *     - Memory is available
 *   Postcondition
 *      - *out is assigned to an iterator that returns the elements of the 
 *      iterator in 
 *      - *out yields entries for which e->type == type
 *
 * DPL_ERR_MEM
 *   Precondition
 *     - iter is allocated
 *     - Memory is not available
 */

int dpl_filter_done (DplIter *in, int done, DplIter **out);
/* Return an iterator that yields either elements done or not done.
 *
 * DPL_OK
 *   Precondition
 *     - in is allocated
 *     - Memory is available
 *   Postcondition
 *      - *out is assigned to an iterator that returns the elements of the 
 *      iterator in 
 *      - *out yields entries for which e->done == done
 *
 * DPL_ERR_MEM
 *   Precondition
 *     - iter is allocated
 *     - Memory is not available
 */


#endif /* DAYPLAN_LIST_H */
