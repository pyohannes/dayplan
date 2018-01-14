/* +
 */

#ifndef DAYPLAN_ENTRY_H
# define DAYPLAN_ENTRY_H

# include <time.h>
# include <stdint.h>


struct DplEntry_;

typedef struct DplEntry_ DplEntry;

typedef enum {
    ENTRY_WORK,
    ENTRY_TASK
} DplEntryType;


int dpl_entry_new (DplEntry **entry, DplEntryType type);
/* Create a new entry object.
 *
 * DPL_OK
 *   Postconditions
 *     - *entry is allocated, all values are initialized to 0
 *     - The type of the entry is set to type.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - not enough memory available
 *   Postconditions
 *     - the content of *entry is undefined
 */

int dpl_entry_free (DplEntry *entry);
/* Free an entry object
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - Memory allocated to entry is freed.
 */

int dpl_entry_name_get (const DplEntry *entry, const char **name);
/* Return the name of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - If a name was set for entry, *name points to it. If no name was set,
 *     *name is set to 0.
 */

int dpl_entry_name_set (DplEntry *entry, const char *name);
/* Set the name of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - enough memory can be allocated
 *   Postcondition
 *     - The content of name is copied and stored as the name of entry. entry
 *     does not obtain ownership of the passed name.
 *
 * DPL_ERR_MEM
 *   Precondition
 *     - entry is allocated
 *     - not enough memory can be allocated
 *   Postcondition
 *     - The name of the entry is 0.
 */

int dpl_entry_begin_get (const DplEntry *entry, time_t *begin);
/* Return the begin time of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - The begin time is stored in begin. Per default it is 0.
 */

int dpl_entry_begin_set (DplEntry *entry, time_t begin);
/* Set the begin time of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - The begin time of the entry is set to begin.
 */

int dpl_entry_desc_get (const DplEntry *entry, const char **desc);
/* Return the description of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - If a description was set for entry, *desc points to it. If no
 *     description was set, *desc is set to 0. 
 */

int dpl_entry_desc_set (DplEntry *entry, const char *desc);
/* Set the description of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - enough memory can be allocated
 *   Postcondition
 *     - The content of desc is copied and stored as the description of the 
 *     entry. entry does not obtain ownership of the passed string.
 *
 * DPL_ERR_MEM
 *   Precondition
 *     - entry is allocated
 *     - not enough memory can be allocated
 *   Postcondition
 *     - The description of the entry is 0.
 */

/* Get the category of an entry.
 *
 * Parameters
 *    @entry An allocated DplEntry.
 *    @cat This is set to point to the category of the entry or to 0 if none 
 *         was set.
 *
 * Returns
 *    DPL_OK always.
 */
int dpl_entry_category_get (const DplEntry *entry, const char **cat);

/* Set the category of an entry.
 *
 * If this entry if of type ENTRY_WORK, if its category is not set and if it is
 * linked to a task, this call returns the category of the linked task.
 *
 * Ownership of cat is not taken, but an internal copy is made.
 *
 * Parameters
 *    @entry An allocated DplEntry.
 *    @cat The new category for the entry.
 *
 * Returns
 *    DPL_OK on success, DPL_ERR_MEM if memory cannot be allocated.
 */
int dpl_entry_category_set (DplEntry *entry, const char *cat);

int dpl_entry_type_get (const DplEntry *entry, DplEntryType *type);
/* Return the type of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - The type is stored in type.
 */

int dpl_entry_type_set (DplEntry *entry, DplEntryType type);
/* Set the type of an entry.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *   Postcondition
 *     - The type of the entry is set to type.
 */

int dpl_entry_work_task_get (const DplEntry *entry, const DplEntry **task);
/* Return the task linked to an entry (work).
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_WORK
 *   Postcondition
 *     - A pointer to the linked task is stored in *task. Per default it is 0.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_WORK
 */

int dpl_entry_work_task_set (DplEntry *entry, const DplEntry *task);
/* Link a task with an entry (work).
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_WORK
 *   Postcondition
 *     - The entry is linked to the task task.
 *     - The entry does not obtain ownership of the task entry object.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_WORK
 */

int dpl_entry_work_end_get (const DplEntry *entry, time_t *end);
/* Return the end time of entry (work9.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_WORK
 *   Postcondition
 *     - The end time of entry is stored in end. Per default this is 0.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_WORK
 */

int dpl_entry_work_end_set (DplEntry *entry, time_t end);
/* Set the end time of entry (work).
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_WORK
 *   Postcondition
 *     - The end time of entry is set to end.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_WORK
 */

int dpl_entry_task_id_get (const DplEntry *entry, uint32_t *id);
/* Return the task id of entry (task).
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_TASK
 *   Postcondition
 *     - The task id of entry is stored in id. Per default this is 0.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_TASK
 */

int dpl_entry_task_id_set (DplEntry *entry, uint32_t id);
/* Set the task id of entry (task).
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_TASK
 *   Postcondition
 *     - The task id of entry is set to id
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_TASK
 */

int dpl_entry_task_done_get (const DplEntry *entry, int *done);
/* Query if an entry (task) is marked as done.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_TASK
 *   Postcondition
 *     - done is set to a value different than 0 if the task is done.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_TASK
 */

int dpl_entry_task_done_set (DplEntry *entry, int done);
/* Mark an entry (task) as done.
 *
 * DPL_OK
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is ENTRY_TASK
 *   Postcondition
 *     - If done contains a value other than 0, the task is marked as done.
 *
 * DPL_ERR_TYPE
 *   Precondition
 *     - entry is allocated
 *     - the type of the entry is not ENTRY_TASK
 */


#endif /* DAYPLAN_ENTRY_H */
