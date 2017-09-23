/* +
 */

#include <string.h>
#include "dpl/time.h"
#include "dpl/defs.h"

int dpl_time_fmt_durance (char *buf, size_t bufsize, const char *format, 
        time_t durance)
{
    int secs, mins, hours, days;
    const char *pos = format;

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
        int printed;

        if (strstr (pos, "%d") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%d", days);
            pos += 2;
        } else if (strstr (pos, "%D") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%d", days);
            pos += 2;
        } else if (strstr (pos, "%h") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%2d", hours);
            pos += 2;
        } else if (strstr (pos, "%H") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%d", 
                    hours + (days * 24));
            pos += 2;
        } else if (strstr (pos, "%m") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%2d", mins); 
            pos += 2;
        } else if (strstr (pos, "%M") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%d", 
                    mins + (hours * 60) + (days * 1440));
            pos += 2;
        } else if (strstr (pos, "%s") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%2d", secs); 
            pos += 2;
        } else if (strstr (pos, "%S") == pos) {
            printed = snprintf (buf + strlen (buf), bufsize, "%d", 
                    secs + (mins * 60) + (hours * 3600) + (days * 86400));
            pos += 2;
        } else {
            printed = snprintf (buf + strlen (buf), bufsize, "%c", *pos);
            pos += 1;
        }

        if ((strlen(buf) + 1 == bufsize) && printed) {
            return DPL_ERR_SIZE;
        }
    }

    return DPL_OK;
}
