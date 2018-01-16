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


/* Parse a date string into a time struct.
 *
 * s can contain either an ISO date (YYYY-mm-dd) or one of the following
 * strings: yesterday, today, tomorrow.
 *
 * The time struct contains the date, with the time set to 00:00:00.
 *
 * Parameters
 *   @s A date string. This must not be 0.
 *   @tm An allocated time struct.
 *
 * Returns
 *   DPL_OK on success. If the string cannot be parsed, DPL_ERR_INPUT is
 *   returned.
 */
int dpl_time_parse_date (const char *s, struct tm *tm);


#endif /* DAYPLAN_TIME_H */
