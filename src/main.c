#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "dpl/dpl.h"
#include "dpl/utils.h"


/* Color codes used for colored terminal output. */
static const char *COLORS[] = {
    "\x1B[0m",
    "\x1B[30;1m",
    "\x1B[33m",
    "\x1B[31m",
    "\x1B[34m",
    "\x1B[36m",
};


#define C_DEFAULT COLORS[0]
#define C_GREY    COLORS[1]
#define C_YELLOW  COLORS[2]
#define C_RED     COLORS[3]
#define C_BLUE    COLORS[4]
#define C_CYAN    COLORS[5]


static const char *usage = "Usage: dayplan [COMMANDS] [OPTIONS]\n"
"\n"
"The following commands are supported:\n"
"\n"
"   work       Print a list of work items.\n"
"   tasks      Print a list of tasks.\n"
"   sum        Print an accumulated sum of time for tasks.\n"
"\n"
"The following options are supported:\n"
"  -h, --help         Print this help text.\n"
"  -f, --file FILE    Read data from given file FILE. This option is\n"
"                     obligatory.\n"
"  -t, --today        Only process today's entries.\n"
"  -d, --date DATE    Only process entries relevant to the date given. This\n"
"                     is ignored when used in conjunction with --date-to,\n"
"                     --date-from or --today.\n"
"  -b, --date-from DATE\n"
"                     Only process entries after and including the given date.\n"
"  -c, --date-to DATE Only process entries before and including the given date.\n"
"  -o, --oneline      Print all task information in one line.\n"
"  -s, --strict       Turn warnings into errors.\n"
"  -g, --group-by-day Group accumulated sums by days, not by tasks.\n"
"";


/* Global struct to hold information from command line options. */
static struct {

    /* Do not consider entries older than tm_from. */
    time_t tm_from;

    /* Do not consider entries newer than tm_to. */
    time_t tm_to;

    /* If set to 1 print entries in a short one-line form. */
    int oneline;

    /* If set to 1 group sums by day. */
    int group_by_day;

    /* If set to 1 treat parser warnings as errors. */
    int strict;

    /* Name of the input file to be used. */
    const char *input;

} options = {
    0, /* tm_from */
    0, /* tm_to */
    0, /* oneline */
    0, /* group_by_day */
    0, /* strict */
    0, /* input */
};


static int dpl_time_to_str (time_t time, char *buf, size_t bufsize)
{
    struct tm tm;
    if (   localtime_r (&time, &tm) == 0
        || asctime_r (&tm, buf) == 0) {
        return DPL_ERR_SYS;
    } 
    buf[strlen (buf) - 1] = 0;

    return DPL_OK;
}


static void dpl_print_indent (const char *s)
{
    printf ("    ");

    while (*s) {
        putchar (*s);

        if (*s++ == '\n') {
            printf ("    ");
        }
    }
}


static void dpl_print_task_oneline (const char *name, const char *begin, 
        const char *durance, int done, int taskid, int print_time)
{
    if (print_time) {
        printf ("%s%s%s %7s  ", C_YELLOW, begin, C_DEFAULT, durance);
    }
    if (taskid) {
            printf ("%s(%s) %s#%d%s ", 
                    done ? C_CYAN : C_RED,
                    done ? "done" : "open",
                    C_GREY,
                    taskid,
                    C_DEFAULT);
    }

    if (name) {
        printf (name);
    }

    printf ("\n");
}


static void dpl_print_task_multiline (const char *name, const char *desc, 
        const char *begin, const char *durance, int done, int taskid, 
        int print_time)
{
    if (taskid) {
        printf ("%sId:      #%d%s\n", C_GREY, taskid, C_DEFAULT);
    }
    if (print_time) {
        printf ("%sDate:    %s%s\n", C_YELLOW, begin, C_DEFAULT);
        printf ("Durance: %s\n", durance);
    }
    if (taskid) {
        printf ("%sStatus:  %s%s\n", 
                done ? C_CYAN : C_RED,
                done ? "done" : "open",
                C_DEFAULT);
    }

    printf ("\n");

    if (name) {
        dpl_print_indent (name);
        printf ("\n\n");
    }

    if (desc) {
        dpl_print_indent (desc);
        printf ("\n\n");
    }
}


static int dpl_print_task (const DplEntry *entry, int print_time) 
{
#define BUFSIZE 1024
    time_t begin, end;
    const char *name;
    char sbegin[BUFSIZE];
    char sdurance[BUFSIZE];
    const DplEntry *task;
    int done = 0;
    uint32_t taskid = 0;

    dpl_entry_begin_get (entry, &begin);
    dpl_entry_name_get (entry, &name);

    if (dpl_entry_work_end_get (entry, &end) == DPL_ERR_TYPE) {
        end = begin;
    }

    if (dpl_time_to_str (begin, sbegin, BUFSIZE) != DPL_OK) {
        fprintf (stderr, "Error: Cannot convert time value: %ld.\n", begin);
        return DPL_ERR_SYS;
    }

    if ((dpl_time_fmt_durance (sdurance, 1024, "%Hh %mm", 
                    end - begin)) != DPL_OK) {
        fprintf (stderr, "Error: Cannot format time value: %ld.\n", 
                begin - end);
        return DPL_ERR_SYS;
    } 

    if (dpl_entry_work_task_get (entry, &task) == DPL_ERR_TYPE) {
        task = entry;
    }

    if (task) {
        dpl_entry_task_done_get (task, &done);
        dpl_entry_task_id_get (task, &taskid);
    } else {
        done = taskid = 0;
    }

    if (options.oneline) {
        dpl_print_task_oneline (name, sbegin, sdurance, done, taskid, 
                print_time);
    } else {
        const char *desc;

        dpl_entry_desc_get (entry, &desc);
        dpl_print_task_multiline (name, desc, sbegin, sdurance, done, taskid, 
                print_time);
    }

    return DPL_OK;
}


static int dpl_period_filter_apply (DplIter **in, DplIter **out)
{
    int ret = DPL_OK;

    if (options.tm_from || options.tm_to) {
        ret = dpl_filter_period (*in, options.tm_from, options.tm_to, 
                out);
    } else {
        *out = *in;
        *in = 0;
    }

    return ret;
}


static int dpl_group_apply (DplIter *iter, DplGroup **group)
{
    if (options.group_by_day) {
        DPL_FORWARD_ERROR (dpl_group_by_day (iter, group));
    } else {
        DPL_FORWARD_ERROR (dpl_group_by_title (iter, group));
    }
    
    return DPL_OK;
}


static int dpl_print_sums (DplList *entries)
{
    DplGroup *first = 0;
    DplIter *i_full = 0;
    DplIter *i_task = 0; 
    DplIter *i_dest = 0;
    DplIter *i_group = 0;
    int ret = DPL_OK;

    DPL_FORWARD_ERROR (dpl_list_iter (entries, &i_full));

    if (   (ret = dpl_filter_type (i_full, ENTRY_WORK, &i_task)) == DPL_OK
        && (ret = dpl_period_filter_apply (&i_task, &i_dest)) == DPL_OK
        && (ret = dpl_group_apply (i_dest, &first)) == DPL_OK) {
        DplGroup *group = first;

        while (group) {
            time_t durance;
            char sdurance[1024];
            const char *name;
    
            if (   (ret = dpl_group_name_get (group, &name)) == DPL_OK
                && (ret = dpl_group_entries_get (group, &i_group)) == DPL_OK
                && (ret = dpl_acc_durance (i_group, &durance)) == DPL_OK
                && (ret = dpl_time_fmt_durance (sdurance, 1024, "%Hh %mm",
                        durance)) == DPL_OK) {
                printf ("%9s %s\n", sdurance, name);

                dpl_iter_free (i_group);
                i_group = 0;
    
                if (dpl_group_next (group, &group) != DPL_OK) {
                    break;
                }
            } else {
                break;
            }
        } 
    }

    if (i_full) dpl_iter_free (i_full);
    if (i_task) dpl_iter_free (i_task);
    if (i_dest) dpl_iter_free (i_dest);
    if (i_group) dpl_iter_free (i_group);
    if (first) dpl_group_free (first);

    return ret;
}


static int dpl_print_task_list (DplList *entries, int done)
{
    DplIter *i_full = 0; 
    DplIter *i_time = 0; 
    DplIter *i_work = 0; 
    DplIter *i_task = 0; 
    DplIter *i_done = 0; 
    DplIter *i_dest = 0;

    const DplEntry *entry;
    int ret = DPL_OK;

    DPL_FORWARD_ERROR (dpl_list_iter (entries, &i_full));

    if (dpl_period_filter_apply (&i_full, &i_time) != DPL_OK) {
        dpl_iter_free (i_full);
        return DPL_ERR_MEM;
    }

    if (done) {
        if (   dpl_filter_type (i_time, ENTRY_WORK, &i_work) != DPL_OK
            || dpl_filter_task_for_work (i_work, &i_task) != DPL_OK
            || dpl_filter_done (i_task, 1, &i_done) != DPL_OK
            || dpl_filter_unique (i_done, &i_dest) != DPL_OK) {
            ret = DPL_ERR_MEM;
        }
    } else {
        if (   dpl_filter_type (i_time, ENTRY_TASK, &i_task) != DPL_OK
            || dpl_filter_done (i_task, 0, &i_dest) != DPL_OK) {
            ret = DPL_ERR_MEM;
        }
    }

    if (ret == DPL_OK) {
        while ((ret = dpl_iter_next (i_dest, &entry)) == DPL_OK) {
            if ((ret = dpl_print_task (entry, 0)) != DPL_OK) {
                break;
            }
        }
        ret = DPL_OK;
    }

    if (i_full) dpl_iter_free (i_full);
    if (i_time) dpl_iter_free (i_time);
    if (i_work) dpl_iter_free (i_work);
    if (i_task) dpl_iter_free (i_task);
    if (i_done) dpl_iter_free (i_done);
    if (i_dest) dpl_iter_free (i_dest);

    return ret;
}


static int dpl_print_tasks (DplList *tasks)
{
    DPL_FORWARD_ERROR (dpl_print_task_list (tasks, 1));
    DPL_FORWARD_ERROR (dpl_print_task_list (tasks, 0));

    return DPL_OK;
}


static int dpl_print_work (DplList *entries)
{
    DplIter *i_full = 0; 
    DplIter *i_work = 0; 
    DplIter *i_time = 0;
    const DplEntry *entry;
    int ret;

    if (   dpl_list_iter (entries, &i_full) == DPL_OK
        && dpl_filter_type (i_full, ENTRY_WORK, &i_work) == DPL_OK
        && dpl_period_filter_apply (&i_work, &i_time) == DPL_OK) {
        while ((ret = dpl_iter_next (i_time, &entry)) == DPL_OK) {
            if ((ret = dpl_print_task (entry, 1)) != DPL_OK) {
                break;
            }
        }
        ret = DPL_OK;
    } else {
        /* nothing else can be returned by the above, except DPL_ERR_MEM and
         * DPL_OK */
        ret = DPL_ERR_MEM;
    }

    if (i_full) {
        dpl_iter_free (i_full);
    }
    if (i_work) {
        dpl_iter_free (i_work);
    }
    if (i_time) {
        dpl_iter_free (i_time);
    }

    return ret;
}


static int dpl_parse_arguments (int argc, char *argv[])
{
    int option_index = 0;
    int c;
    struct tm tm;
    static struct option long_options[] = {
        { "today", 0, 0, 't' },
        { "date", 1, 0, 'd' },
        { "date-from", 1, 0, 'b' },
        { "date-to", 1, 0, 'c' },
        { "oneline", 0, 0, 'o' },
        { "file", 1, 0, 'f' },
        { "group-by-day", 0, 0, 'g' },
        { "strict", 0, 0, 's' },
        { "help", 0, 0, 'h' },
        { 0, 0, 0, 0 }
    };

#define PARSE_DATE { \
    time_t now = time (0); \
    int ret; \
    ret = sscanf (optarg, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday); \
    if (ret != 3) { \
        fprintf (stderr, "Error: Invalid date format: %s\n", optarg); \
        return DPL_ERR_INPUT; \
    } \
    tm.tm_mon -= 1; \
    tm.tm_year -= 1900; \
    tm.tm_sec = tm.tm_min = tm.tm_hour = 0; \
    tm.tm_isdst = daylight; \
}

    while ((c = getopt_long (argc, argv, "thosrad:b:c:f:g", long_options, 
                    &option_index)) != -1) {
        switch (c) {
            case 'f':
                options.input = optarg;
                break;
            case 't':
                options.tm_from = time (0);
                localtime_r (&options.tm_from, &tm);
                tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
                tm.tm_isdst = daylight;
                options.tm_from = mktime (&tm);
                options.tm_to = options.tm_from + (3600 * 24) - 1;
                break;
            case 'b':
                PARSE_DATE
                options.tm_from = mktime (&tm);
                if (!options.tm_to) {
                    options.tm_to = LONG_MAX;
                }
                break;
            case 'c':
                PARSE_DATE
                options.tm_to = mktime (&tm) + (3600 * 24) - 1;
                break;
            case 'd':
                PARSE_DATE
                tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
                options.tm_from = mktime (&tm);
                options.tm_to = options.tm_from + (3600 * 24) - 1;
                break;
            case 'h':
                printf (usage);
                return 0;
            case 'o':
                options.oneline = 1;
                break;
            case 'g':
                options.group_by_day = 1;
                break;
            case 's':
                options.strict = 1;
                break;
            case '?':
                /* getopt gives error information */
                return DPL_ERR_INPUT;
        }
    }

    return DPL_OK;
}


static int dpl_check_color_output ()
{
    if (!isatty (1)) {
        size_t i;

        for (i = 0; i < sizeof (COLORS) / sizeof (COLORS[0]); i++) {
            COLORS[i] = "";
        }
    }

    return DPL_OK;
}


static int dpl_parse_main (DplList **entries)
{
    if (options.input) {
        DPL_FORWARD_ERROR (
                dpl_parse_file (options.input, entries, options.strict));
    } else {
        DPL_FORWARD_ERROR (
                dpl_parse (stdin, "<stdin>", entries, options.strict));
    }

    return DPL_OK;
}


static int dpl_process_commands (int argc, char *argv[], DplList *entries)
{
    while (optind < argc) {
        if (strcmp (argv[optind], "work") == 0) {
            DPL_FORWARD_ERROR (dpl_print_work (entries));
        } else if (strcmp (argv[optind], "tasks") == 0) {
            DPL_FORWARD_ERROR (dpl_print_tasks (entries));
        } else if (strcmp (argv[optind], "sum") == 0) {
            DPL_FORWARD_ERROR (dpl_print_sums (entries));
        } else {
            fprintf (stderr, usage);
            return DPL_ERR_INPUT;
        } 
        optind += 1;
    }

    return DPL_OK;
}


static int dpl_report_error (int errorcode)
{
    const char *msg;

    switch (errorcode) {
        case DPL_ERR_MEM:
            msg = "Out of memory";
            break;
        case DPL_ERR_SIZE:
            msg = "Internal overflow";
            break;
        default:
            msg = "Unknown error code";
            break;
    }

    fprintf (stderr, "Error: %s.\n", msg);

    return DPL_OK;
}


int main (int argc, char *argv[])
{
    int ret = 1;
    DplList *entries;

    dpl_check_color_output ();

    if (dpl_parse_arguments (argc, argv) != DPL_OK) {
        return 1;
    }

    if (dpl_parse_main (&entries) != DPL_OK) {
        return 1;
    }

    ret = dpl_process_commands (argc, argv, entries);
    
    if (ret != DPL_OK) {
        dpl_report_error (ret);
    }

    dpl_list_free (entries, 1);

    return (ret != DPL_OK);
}
