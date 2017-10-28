/* +
 */

#ifndef DAYPLAN_PARSER_H
# define DAYPLAN_PARSER_H

# include "dpl/list.h"


int dpl_parse (const char *filename, DplList **list, int strict);
/* Parse a dayplan data file.
 *
 * Preconditions
 *   - filename is not 0
 *   - list is not 0
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - memory cannot be allocated
 *
 * Preconditions
 *   - memory can be allocated
 *
 * DPL_ERR_IO
 *   Preconditions
 *     - The file denoted by filename cannot be opened for reading.
 *
 * Preconditions
 *   - The file denoted by filename can be opened for reading.
 * 
 * DPL_ERR_SYNTAX
 *   Preconditions
 *     - The data file contains a syntax error.
 *   Postconditions
 *     - An error message is reported via dpl_error.
 *
 * DPL_ERR_INPUT
 *   Preconditions
 *     - The data file contains a semantic error.
 *   Postconditions
 *     - An error message is reported via dpl_error.
 *
 * DPL_ERR_INPUT
 *   Preconditions
 *     - The data file contains a semantic problem that leads to a warning.
 *     - strict is not 0
 *   Postconditions
 *     - An error message is reported via dpl_error.
 *
 * Postconditions
 *  - *list contains a list of work and task entries
 *  - *list is sorted
 *  - work and task entries in *list are linked accordingly to the data
 *
 * DPL_OK
 *   Preconditions
 *     - The data file contains a semantic problem that leads to a warning.
 *     - strict is 0
 *
 * DPL_OK
 */


#endif /* DAYPLAN_PARSER_H */
