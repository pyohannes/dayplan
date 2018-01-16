/* +
 */


#include <string.h>
#include <stdio.h>
#include "dpl/time.h"
#include "dpl/defs.h"


int dpl_time_fmt_durance (char *buf, size_t bufsize, const char *format, 
        time_t durance)
{
    int secs, mins, hours, days;
    const char *pos = format;
    size_t bufpos = 0;
    const char *fmt;
    int value;

    secs = mins = hours = days = 0;

    if (durance) {
        secs = durance % 60;
        durance -= secs;
        durance /= 60;
    }
    
    if (durance) {
        mins = durance % 60;
        durance -= mins;
        durance /= 60;
    }
    
    if (durance) {
        hours = durance % 24;
        durance -= hours;
        days = durance / 24;
    }

    strncpy (buf, "", bufsize);

    while (pos && *pos) {

        if (bufpos >= (bufsize - 1)) {
            return DPL_ERR_SIZE;
        }

        /* no format specifier */
        if (*pos != '%') {
            buf[bufpos++] = *pos++;
            buf[bufpos] = '\0';
            continue;
        }

        pos += 1;

        /* '%' is the last character in the string */
        if (!*pos) {
            buf[bufpos++] = '%';
            buf[bufpos] = '\0';
            break;
        }

        /* check character after '%' */
        switch (*pos) {
            case 'd': 
                fmt = "%d"; value = days; 
                break;
            case 'D': 
                fmt = "%d"; value = days;
                break;
            case 'h': 
                fmt = "%2d"; value = hours; 
                break;
            case 'H': 
                fmt = "%d"; value = hours + (days * 24);
                break;
            case 'm': 
                fmt = "%2d"; value = mins; 
                break;
            case 'M': 
                fmt = "%d", value = mins + (hours * 60) + (days * 1440);
                break;
            case 's': 
                fmt = "%2d"; value = secs; 
                break;
            case 'S': 
                fmt = "%d"; 
                value =  secs + (mins * 60) + (hours * 3600) + (days * 86400);
                break;
            default:
                 fmt = "%c"; value = *pos;
                 break;
        }

        /* write and check for overflow */
        bufpos += snprintf (buf + bufpos, bufsize - bufpos, fmt, value);
        if (bufpos >= bufsize) {
            return DPL_ERR_SIZE;
        }

        pos += 1;
    }

    return DPL_OK;
}


int dpl_time_parse_date (const char *s, struct tm *tm)
{ 
    time_t now; 
    int ret; 

    if (strcmp (s, "yesterday") == 0) { 
        now = time (0) - 3600 * 24; 
        localtime_r (&now, tm); 
    } else if (strcmp (s, "today") == 0) { 
        now = time (0); 
        localtime_r (&now, tm); 
    } else if (strcmp (s, "tomorrow") == 0) { 
        now = time (0) + 3600 * 24; 
        localtime_r (&now, tm); 
    } else { 
        if (sscanf (s, "%d-%d-%d", 
                    &tm->tm_year, &tm->tm_mon, &tm->tm_mday) != 3) {
            fprintf (stderr, "Error: Invalid date format: %s\n", s); 
            return DPL_ERR_INPUT; 
        } 
        tm->tm_mon -= 1; 
        tm->tm_year -= 1900; 
    }

    tm->tm_sec = tm->tm_min = tm->tm_hour = 0; 
    tm->tm_isdst = daylight; 

    return DPL_OK;
}
