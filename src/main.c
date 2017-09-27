#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "dpl/dpl.h"


static const char *usage = "Usage: dayplan [OPTIONS] FILE\n"
"  -h, --help         Print this help text.\n"
"  -t, --today        Only process today's entries.\n"
"  -d, --date         Only process entries relevant to the date given. This\n"
"                     is ignored when used in conjunction with --date-to,\n"
"                     --date-from or --today.\n"
"  -b, --date-from    Only process entries after and including the given date.\n"
"  -c, --date-to      Only process entries before and including the given date.\n"
"  -o, --oneline      Print all task information in one line.\n"
"  -s, --show-summary Combines --show-tasks and --show-refs.\n"
"  -a, --show-tasks   Show tasks.\n"
"  -r, --show-refs    Show todo references.\n"
"";


static struct {
    time_t tm_from;
    time_t tm_to;
    int oneline;
    int show_summary;
    int show_tasks;
    int show_refs;
    int show_title;
    const char *input;
} options = {
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0
};


int print_task (DplTask *task, int print_time_info) 
{
    time_t begin, end;
    const char *title;
    const char *desc;
    char sbegin[1024];
    char sdurance[1024];
    int ret;

    ret = dpl_task_begin_get (task, &begin);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task begin time.\n");
        return ret;
    }

    ret = dpl_task_end_get (task, &end);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task end time.\n");
        return ret;
    }

    ret = dpl_task_title_get (task, &title);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task title.\n");
        return ret;
    }

    ret = dpl_task_desc_get (task, &desc);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task description.\n");
        return ret;
    } 

    if (!ctime_r (&begin, sbegin)) {
        fprintf (stderr, "Error: Cannot convert time value: %ld.\n", begin);
        return ret;
    } 
    sbegin[strlen (sbegin) - 1] = 0;

    if ((dpl_time_fmt_durance (sdurance, 1024, "%Hh %mm", 
                    end - begin)) != DPL_OK) {
        fprintf (stderr, "Error: Cannot format time value: %ld.\n", 
                begin - end);
        return DPL_ERR_SYS;
    } 

    if (options.oneline) {
        if (print_time_info) {
            printf ("%s  %s  %s\n", sbegin, sdurance, title ? title : "");
        } else {
            printf ("    %s\n", title ? title : "");
        }
    } else {
        if (print_time_info) {
            printf ("Date:    %s\n", sbegin);
            printf ("Durance: %s\n\n", sdurance);
        }

        if (title) {
            printf ("    %s\n\n", title);
        }

        if (desc) {
            printf ("    %s\n\n", desc);
        }
    }

    return DPL_OK;
}


int print_ref_list (DplTaskList *tasks, const char *title, int done, 
        int undone)
{
    DplTaskListIter *iter_full, *iter_refs, *iter_done, *iter_undone, *iter;
    DplTaskListFilter *ftoday, *fref, *fundone, *fdone;
    DplTask *task;
    int ret;

    ret = dpl_tasklist_iter (tasks, &iter_full);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task list iterator.\n");
        return ret;
    }

    ret = dpl_tasklist_filter_refs (&fref);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain reference filter.\n");
        return ret;
    }

    ret = dpl_tasklist_filter (iter_full, fref, &iter_refs);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot apply reference filter.\n");
        return ret;
    }

    if (done) {
        ret = dpl_tasklist_filter_done (&fdone);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot allocate done filter.\n");
            return ret;
        }
    
        ret = dpl_tasklist_filter (iter_refs, fdone, &iter_done);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot assign done filter.\n");
            return ret;
        }
    } else {
        iter_done = iter_refs;
    }

    if (undone) {
        ret = dpl_tasklist_filter_undone (&fundone);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot allocate undone filter.\n");
            return ret;
        }
    
        ret = dpl_tasklist_filter (iter_done, fundone, &iter_undone);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot assign undone filter.\n");
            return ret;
        }
    } else {
        iter_undone = iter_done;
    }

    if (options.tm_from || options.tm_to) {
        ret = dpl_tasklist_filter_period (options.tm_from, options.tm_to, 
                &ftoday);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot allocate task list filter.\n");
            return ret;
        }
    
        ret = dpl_tasklist_filter (iter_undone, ftoday, &iter);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot assign task list filter.\n");
            return ret;
        }
    } else {
        /* FIXME: careful when freeing! */
        iter = iter_refs;
    }

    if (options.show_title) {
        printf ("\n%s\n", title);
    }

    while ((ret = dpl_tasklistiter_next (iter, &task)) == DPL_OK) {
        print_task (task, 0);
    }

    return DPL_OK;
}


int print_task_list (DplTaskList *tasks)
{
    DplTaskListIter *iter_full, *iter_tasks, *iter;
    DplTaskListFilter *ftoday, *ftasks;
    DplTask *task;
    int ret;

    ret = dpl_tasklist_iter (tasks, &iter_full);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task list iterator.\n");
        return ret;
    }

    ret = dpl_tasklist_filter_tasks (&ftasks);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot obtain task filter.\n");
        return ret;
    }

    ret = dpl_tasklist_filter (iter_full, ftasks, &iter_tasks);
    if (ret != DPL_OK) {
        fprintf (stderr, "Error: Cannot apply task filter.\n");
        return ret;
    }

    if (options.tm_from || options.tm_to) {
        ret = dpl_tasklist_filter_period (options.tm_from, options.tm_to, 
                &ftoday);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot allocate task list filter.\n");
            return ret;
        }
    
        ret = dpl_tasklist_filter (iter_tasks, ftoday, &iter);
        if (ret != DPL_OK) {
            fprintf (stderr, "Error: Cannot assign task list filter.\n");
            return ret;
        }
    } else {
        /* FIXME: careful when freeing! */
        iter = iter_tasks;
    }

    if (options.show_title) {
        printf ("\nTASKS\n");
    }

    while ((ret = dpl_tasklistiter_next (iter, &task)) == DPL_OK) {
        print_task (task, 1);
    }

    return DPL_OK;
}


int parse_arguments (int argc, char *argv[])
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
        { "show-summary", 0, 0, 's' },
        { "show-tasks", 0, 0, 'a' },
        { "show-refs", 0, 0, 'r' },
        { "help", 0, 0, 'h' },
        { 0, 0, 0, 0 }
    };

#define PARSE_DATE { \
    time_t now = time (0); \
    int ret; \
    localtime_r (&now, &tm); \
    ret = sscanf (optarg, "%u-%u-%u", &tm.tm_year, &tm.tm_mon, &tm.tm_mday); \
    if (ret != 3) { \
        fprintf (stderr, "Error: Invalid date format: %s\n", optarg); \
        return DPL_ERR_INPUT; \
    } \
    tm.tm_mon -= 1; \
    tm.tm_year -= 1900; \
    tm.tm_sec = tm.tm_min = tm.tm_hour = 0; \
}

    while ((c = getopt_long (argc, argv, "thosradbc", long_options, 
                    &option_index)) != -1) {
        switch (c) {
            case 't':
                options.tm_from = time (0);
                localtime_r (&options.tm_from, &tm);
                tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
                printf ("### %s\n", asctime (&tm));
                options.tm_from = mktime (&tm);
                options.tm_to = options.tm_from + (3600 * 24) - 1;
                break;
            case 'b':
                PARSE_DATE
                options.tm_from = mktime (&tm);
                break;
            case 'c':
                PARSE_DATE
                options.tm_to = mktime (&tm) + (3600 * 24) - 1;
                break;
            case 'd':
                PARSE_DATE
                tm.tm_sec = tm.tm_min = tm.tm_hour = 0;
                printf ("### %s\n", asctime (&tm));
                options.tm_from = mktime (&tm);
                options.tm_to = options.tm_from + (3600 * 24) - 1;
                break;
            case 'h':
                printf (usage);
                return 0;
            case 'o':
                options.oneline = 1;
                break;
            case 'a':
                options.show_tasks = 1;
                break;
            case 'r':
                options.show_refs = 1;
                break;
            case 's':
                options.show_tasks = options.show_refs = 1;
                break;
            case '?':
                fprintf (stderr, "Error: Invalid arguments.\n%s", usage);
                return DPL_ERR_INPUT;
        }
    }

    printf ("### from: %ld, to: %ld\n", options.tm_from, options.tm_to);

    options.show_title = (options.show_refs + options.show_tasks) > 1;

    if (optind != (argc - 1)) {
        fprintf (stderr, usage);
        return DPL_ERR_INPUT;
    }

    options.input = argv[optind];

    return DPL_OK;
}


int main (int argc, char *argv[])
{
    int ret;
    DplTaskList *tasks;

    ret = parse_arguments (argc, argv);
    if (ret != DPL_OK) {
        return 1;
    }

    ret = dpl_parse (options.input, &tasks);
    if (ret != DPL_OK) {
        /* dpl_parse prints an error */
        return 1;
    }

    if (options.show_tasks) {
        ret = print_task_list (tasks);
        if (ret != DPL_OK) {
            return 1;
        }
    }

    if (options.show_refs) {
        ret = print_ref_list (tasks, "TODO DONE", 1, 0);
        if (ret != DPL_OK) {
            return 1;
        }
        ret = print_ref_list (tasks, "TODO OPEN", 0, 1);
        if (ret != DPL_OK) {
            return 1;
        }
    }

    dpl_tasklist_free (tasks, 1);

    return 0;
}
