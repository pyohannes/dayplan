/* +
 */

#ifndef DAYPLAN_TIME_H
# define DAYPLAN_TIME_H


# include <time.h>

int dpl_time_fmt_durance (char *buf, size_t bufsize, const char *format, 
        time_t durance);
/* Convert a durance to a formatted C string.
 *
 * DPL_OK
 *   Precondition
 *     - buf is character buffer of size bufsize
 *     - format is format string that can contain placeholders described below
 *     - durance is a time durance in seconds
 *     - buf is large enough to hold the format string with replaced 
 *     placeholders
 *   Postcondition
 *     - buf holds the format string with replaced placeholders, where possible
 *     placeholders are:
 *       %d, %D: The number of days in durance.
 *       %h: The hours in durance.
 *       %H: The absolute number of hours in durance, including the days.
 *       %m: The minutes in durance.
 *       %H: The absolute number of minutes in durance, including the days and 
 *           hours.
 *       %s: The seconds in durance.
 *       %S: The absolute numbers of seconds in durance. This equals the given 
 *           durance.
 *
 * DPL_ERR_SIZE
 *   Precondition
 *     - buf is too small to hold the format string with replaced placeholders
 *   Postcondition
 *     - The contents of buf are undefined.
 */

#endif /* DAYPLAN_TIME_H */

