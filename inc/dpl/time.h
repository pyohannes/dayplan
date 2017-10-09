/* +
 */

#ifndef DAYPLAN_TIME_H
# define DAYPLAN_TIME_H


# include <time.h>

int dpl_time_fmt_durance (char *buf, size_t bufsize, const char *format, 
        time_t durance);
/* Convert a durance to a formatted C string.
 *
 * A given durance is, according to a given format string, converted to a C
 * string. The string is stored in buf, where bufsize denotes the size of buf.
 *
 * The durance is given in seconds.
 *
 * The format string can contain the following placeholders:
 *
 *  %d, %D: The number of days in durance.
 *  %h: The hours in durance.
 *  %H: The absolute number of hours in durance, including the days.
 *  %m: The minutes in durance.
 *  %H: The absolute number of minutes in durance, including the days and hours.
 *  %s: The seconds in durance.
 *  %S: The absolute numbers of seconds in durance. This equals the given 
 *      durance.
 *
 * Any other format string specifiers are printed as given.
 *
 * On success DPL_OK is returned. If the given buffer is too small,
 * DPL_ERR_SIZE is returned.
 */

#endif /* DAYPLAN_TIME_H */

