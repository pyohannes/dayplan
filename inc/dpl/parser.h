/* +
 */

#ifndef DAYPLAN_PARSER_H
# define DAYPLAN_PARSER_H

# include "dpl/tasklist.h"


int dpl_parse (const char *filename, DplTaskList **list, int strict);


#endif /* DAYPLAN_PARSER_H */
