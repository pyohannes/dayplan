%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "dpl/parser.h"
#include "dpl/error.h"
#include "dpl/defs.h"
#include "dpl/utils.h"


typedef struct {
    /* INPUT */

    /* The file that is read. */
    FILE *file;          

    /* The name of the file that is read. */
    const char *filename; 

    /* If not 0, all warnings are errors. */
    int strict;           

    /* PARSER */

    /* The list of parsed entries. */
    DplList *entries;     

    /* The entry that currently is parsed. */
    DplEntry *current;    

    /* The date of the currently active day and the time of the current task.
     */
    struct tm tm;

    /* STRING CACHE */

    /* Buffer that holds the string that is currently parsed. */
    char *text;

    /* The allocated size of the buffer. */
    size_t textlen;

    /* The position of the last read character in the buffer. */
    size_t textpos;

    /* LEXER */
    
    /* The last character read. */
    int c;

    /* The number of the line the last character was read from. */
    size_t lineno;

    /* The number of whitespaces of the first indent level. 0 if the current
     * line is not indented. */
    int indent1; 

    /* The number of whitespaces of the second indent level. 0 if the current
     * line is not intented or is in indent level 1. */
    int indent2;

    /* Lexer state. If this is not 0, the lexer returns a DEINDENT. */
    int cache_deindent;

    /* Lexer state. If this is not 0, the lexer expects an indent. */
    int cache_parseindent;

} DplParseContext;


/* forward declaration */
static int yylex (DplParseContext *ctx);


/* Adds a character to the string cache. 
 * 
 * Preconditions
 *   - DPL_ctx is of type DplParseContext *
 *   - DPL_ctx is allocated
 *   - 0 <= DPL_ctx->textlen < DPL_ctx->textpos 
 *   - memory can be allocated
 * Postcondition
 *   - DPL_ctx->text[DPL_ctx->textpos] = DPL_i
 *   - DPL_ctx->textpos += 1
 *
 * Preconditions
 *   - DPL_ctx is of type DplParseContext *
 *   - DPL_ctx is allocated
 *   - DPL_ctx->textlen == DPL_ctx->textpos 
 *   - memory can be allocated
 * Postcondition
 *   - The buffer DPL_ctx->text is enlarged, DPL_ctx->textlen is set
 *     accordingly
 *   - DPL_ctx->text[DPL_ctx->textpos] = DPL_i
 *   - DPL_ctx->textpos += 1
 *
 * Preconditions
 *   - DPL_ctx is of type DplParseContext *
 *   - DPL_ctx is allocated
 *   - DPL_ctx->textlen == DPL_ctx->textpos 
 *   - memory cannot be allocated
 * Postcondition
 *   - DPL_ERR_MEM is returned from the calling function
 */
# define DPL_ADD_CHAR(DPL_ctx, DPL_i) \
    if ((DPL_ctx)->textpos >= (DPL_ctx)->textlen) { \
        (DPL_ctx)->textlen = 80 + (DPL_ctx)->textlen * 2; \
        (DPL_ctx)->text = realloc ((DPL_ctx)->text, (DPL_ctx)->textlen); \
        if (!(DPL_ctx)->text) { \
            return DPL_ERR_MEM; \
        } \
    } \
    (DPL_ctx)->text[(DPL_ctx)->textpos++] = DPL_i;


/* Report an error.
 *
 * DPL_ERR_SYNTAX
 *   Preconditions
 *     - ctx is allocated
 *     - s is not 0
 *   Postconditions
 *     - The passed error message and the file name and line number is reported
 *       via dpl_error.
 */
static int dpl_parse_error (DplParseContext *ctx, const char *s)
{
    dpl_error ("%s:%u: error: %s\n", ctx->filename, ctx->lineno, s);

    return DPL_ERR_SYNTAX;
}


/* bison expects a function yyerror. */
#define yyerror dpl_parse_error


/* Report a warning.
 *
 * Preconditions
 *   - ctx is allocated
 *   - s is not 0
 * Postconditions
 *   - The passed warning message and the file name and line number is reported
 *     via dpl_error.
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->strict is 0
 *
 * DPL_ERR_SYNTAX
 *   Preconditions
 *     - ctx->strict is not 0
 */
static int dpl_parse_warning (DplParseContext *ctx, const char *s)
{
    if (ctx->strict) {
        return dpl_parse_error (ctx, s);
    } else {
        dpl_error ("%s:%u: warning: %s\n", ctx->filename, ctx->lineno, s);
    }

    return DPL_OK;
}


/* Set correct end times for all work entries.
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->entries is allocated and initialized
 *     - enough memory can be allocated
 *     - ctx->entries ends with an end marker
 *   Postconditions
 *     - For all entries of type ENTRY_WORK, the end time of the entry is set
 *       to the begin time of the following entry. 
 *
 * DPL_ERR_INPUT
 *   Preconditions
 *     - ctx->entries is allocated and initialized
 *     - enough memory can be allocated
 *     - ctx->entries does not end with an end marker
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - not enough memory can be allocated
 */
static int dpl_parse_end_set (DplParseContext *ctx)
{
    DplIter *iter;
    const DplEntry *curr, *next;
    const char *name;

    DPL_FORWARD_ERROR (dpl_list_iter (ctx->entries, &iter));
    if (dpl_iter_next (iter, &curr) != DPL_OK) {
        /* empty task list */
        return DPL_OK;
    }

    while (dpl_iter_next (iter, &next) == DPL_OK) {
        time_t begin;
        DplEntryType type;

        /* set a correct end time for work items */
        dpl_entry_type_get (curr, &type);
        if (type == ENTRY_WORK) {
            dpl_entry_begin_get (next, &begin);
            dpl_entry_work_end_set ((DplEntry *)curr, begin);
        }

        curr = next;
    }

    dpl_entry_name_get (curr, &name);
    if (name) {
        /* the last entry (curr) needs to be a nameless end marker */
        dpl_parse_error (ctx, "No end marker task found");
        return DPL_ERR_INPUT;
    }

    dpl_iter_free (iter);

    return DPL_OK;
}


/* Remove all end marker entries.
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->entries is allocated and initialized
 *     - enough memory can be allocated
 *   Postconditions
 *     - All end marker entries in ctx->entries are removed from ctx->entries 
         and freed.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - not enough memory can be allocated
 */
static int dpl_parse_marker_remove (DplParseContext *ctx)
{
    DplIter *iter;
    const DplEntry *curr;
    const char *name;

    DPL_FORWARD_ERROR (dpl_list_iter (ctx->entries, &iter));

    while (dpl_iter_next (iter, &curr) == DPL_OK) {
        dpl_entry_name_get (curr, &name);
        if (!name) {
            dpl_list_remove (ctx->entries, curr);
            dpl_entry_free ((DplEntry *)curr);
        }
    }

    dpl_iter_free (iter);

    return DPL_OK;
}


/* Check if a task id was already defined.
 *
 * Preconditions
 *  - ctx, entry and tasks_open are allocated
 *  - tasks_open contains only entries of type ENTRY_TASK
 *  - entry is of type ENTRY_TASK
 *
 * DPL_OK
 *   Preconditions
 *     - tasks_open does not contain a task with the same id as entry
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->strict is 0
 *     - tasks_open contains a task with the same id as entry
 *   Postconditions
 *     - a warning message is given
 *
 * DPL_ERR_INPUT
 *   Preconditions
 *     - ctx->strict is not 0
 *     - tasks_open contains a task with the same id as entry
 *   Postconditions
 *     - an error message is given
 */
static int dpl_parse_link_check_duptask (DplParseContext *ctx, 
    const DplEntry *entry, DplList *tasks_open)
{
    DplIter *iter;
    const DplEntry *task;
    uint32_t taskid;

    dpl_entry_task_id_get (entry, &taskid);

    DPL_FORWARD_ERROR (dpl_list_iter (tasks_open, &iter));
    while (dpl_iter_next (iter, &task) == DPL_OK) {
        uint32_t id;

        dpl_entry_task_id_get (task, &id);
        if (id == taskid) {
            char errmsg[1024];
            snprintf (errmsg, 1024, "Duplicate task id #%d", id);
            if (dpl_parse_warning (ctx, errmsg) != DPL_OK) {
                dpl_iter_free (iter);
                return DPL_ERR_INPUT;
            }
        }
    }

    dpl_iter_free (iter);            

    DPL_FORWARD_ERROR (dpl_list_push (tasks_open, entry));

    return DPL_OK;
}


/* Extract a task id and done flag from a work name.
 *
 * DPL_OK
 *   Preconditions
 *     - The name of the work matches '^#[0-9]*'
 *   Postconditions
 *     - taskid is set to the number '[0-9]*' in the work name
 *     - done is set to 0
 *
 * DPL_OK
 *   Preconditions
 *     - The name of the work matches '^+#[0-9]*'
 *   Postconditions
 *     - taskid is set to the number '[0-9]*' in the work name
 *     - done is set to 1
 *
 * DPL_OK
 *   Preconditions
 *     - The name of the work does not match any of the above
 *   Postconditions
 *     - taskid and done are set to 0
 */
static int dpl_parse_link_parse_workname (DplParseContext *ctx, 
    const DplEntry *work, uint32_t *taskid, int *done)
{
    const char *name;

    *taskid = 0;
    *done = 0;

    dpl_entry_name_get (work, &name);

    if (name && (name[0] == '#' || name[0] == '+')) {
        if (sscanf (name, "+#%u", taskid)) {
            *done = 1;
        } else if (sscanf (name, "#%u", taskid)) {
        } else {
            *taskid = 0;
        }
    }

    return DPL_OK;
}


/* Try to link a work entry to an open task entry.
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated
 *
 * Preconditions
 *   - ctx, work and tasks_open are allocated
 *   - work is of type ENTRY_WORK
 *   - all items in tasks_open are of type ENTRY_TASK
 *   - Memory can be allocated
 * 
 * DPL_OK
 *   Precondition
 *     - tasks_open contains a task entry with task id id_ref
 *     - done is 0
 *   Postconditions
 *     - work is linked to the task with id id_ref
 *
 * DPL_OK
 *   Precondition
 *     - tasks_open contains a task entry with task id id_ref
 *     - done is not 0
 *   Postconditions
 *     - work is linked to the task with id id_ref
 *     - the task with the id id_ref is set to done
 *     - the task with the id id_ref is removed from tasks_open
 */
static int dpl_parse_link_work (DplParseContext *ctx, const DplEntry *work,
    uint32_t id_ref, int done, DplList *tasks_open)
{
    DplIter *iter;
    const DplEntry *task;

    DPL_FORWARD_ERROR (dpl_list_iter (tasks_open, &iter));
    while (dpl_iter_next (iter, &task) == DPL_OK) {
        uint32_t id;

        dpl_entry_task_id_get (task, &id);
        if (id_ref == id) {
            dpl_entry_work_task_set ((DplEntry *)work, task);
            if (done) {
                dpl_entry_task_done_set ((DplEntry *)task, 1);
                dpl_list_remove (tasks_open, task);
            }
            break;
        }
    }

    dpl_iter_free (iter);

    return DPL_OK;
}


/* Link tasks and work items and check for duplicate tasks.
 *
 * Preconditions
 *   - ctx is allocated
 *   - ctx->entries contains a list of entry objects
 *   - ctx->entries is sorted by begin time
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated
 *
 * Preconditions
 *   - Memory can be allocated
 *
 * DPL_ERR_INPUT
 *   Preconditions
 *     - ctx->strict is not 0
 *     - ctx->entries contains two task entries with the same id
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->strict is 0 or ctx->entries contains no two tasks with the same
 *       id
 *   Postconditions
 *     - Work entries in ctx->entries with a task id in the name are linked to
 *       the task with the id if it is not done at the begin time of the work
 *       entry
 *     - Task entries in ctx->entries are set to done if a work entry is found
 *       that marks the task as done and the task is not done at the begin time
 *       of the work entry
 */
static int dpl_parse_link_entries (DplParseContext *ctx)
{
    DplIter *iter;
    const DplEntry *entry;
    DplList *tasks_open;
    int ret = DPL_OK;

    DPL_FORWARD_ERROR (dpl_list_new (&tasks_open));

    if ((ret = dpl_list_iter (ctx->entries, &iter)) != DPL_OK) {
        dpl_list_free (tasks_open, 0);
        return ret;
    }

    while (dpl_iter_next (iter, &entry) == DPL_OK) {
        uint32_t taskid;

        if (dpl_entry_task_id_get (entry, &taskid) == DPL_ERR_TYPE) {
            /* entry is a work */
            int done;

            dpl_parse_link_parse_workname (ctx, entry, &taskid, &done);

            if (!taskid) {
                continue;
            }

            ret = dpl_parse_link_work (ctx, entry, taskid, done, tasks_open);
        } else {
            ret = dpl_parse_link_check_duptask (ctx, entry, tasks_open);
        }

        if (ret != DPL_OK) {
            goto dpl_parse_link_entries_error;
        }
    }

dpl_parse_link_entries_error:
    dpl_iter_free (iter);
    dpl_list_free (tasks_open, 0);

    return ret;
}


/* Create a new entry.
 *
 * Preconditions
 *   - ctx is allocated
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - memory cannot be allocated
 *
 * Preconditions
 *   - memory can be allocated
 *
 * Postconditions
 *   - ctx->current is assigned to a newly allocated entry object
 *   - the begin time of ctx->current is set to ctx->tm
 *   - ctx->current is appended to ctx->entries
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->textpos is 0
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->textpos is not 0
 *   Postconditions
 *     - the name of ctx->current is assigned to ctx->text[0:ctx->textpos]
 *     - whitspaces in trimmed from ctx->text
 */
static int dpl_parse_entry_new (DplParseContext *ctx, DplEntryType type)
{
    DPL_FORWARD_ERROR (dpl_entry_new (&ctx->current, type));
    ctx->tm.tm_isdst = -1;
    DPL_FORWARD_ERROR (dpl_entry_begin_set (ctx->current, mktime (&ctx->tm)));
    if (ctx->textpos) {
        DPL_ADD_CHAR(ctx, 0);
        DPL_FORWARD_ERROR (dpl_entry_name_set (ctx->current, 
            dpl_skip_whitespaces (ctx->text)));
    }
    DPL_FORWARD_ERROR (dpl_list_push (ctx->entries, ctx->current));

    return DPL_OK;
}


/* Set the description of the current entry.
 *
 * Preconditions
 *   - ctx and ctx->current are allocated
 *
 * DPL_ERR_MEM
 *   Preconditions
 *     - Memory cannot be allocated
 *
 * Preconditions
 *   - Memory can be allocated
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->textpos is 0
 *
 * DPL_OK
 *   Preconditions
 *     - ctx->textpos is not 0
 *   Postconditions
 *     - the description of ctx->current is assigned to 
 *       ctx->text[0:ctx->textpos]
 *     - whitspaces in trimmed from ctx->text
 */ 
static int dpl_parse_desc_set (DplParseContext *ctx)
{
    if (ctx->textpos) {
        DPL_ADD_CHAR(ctx, 0);
        DPL_FORWARD_ERROR (dpl_entry_desc_set (ctx->current,
            dpl_skip_whitespaces (ctx->text)));
    }
    return DPL_OK;
}


%}

%start days
%token DIGIT LETTER COLON HYPHEN NEWLINE SPACE TAB HASH INDENT DEINDENT
%param {DplParseContext *ctx}

%define parse.error verbose

%%
days : 
     | day days
     {
     }
     ;

day : date paddednewline INDENT items
    ;

items : DEINDENT
      | NEWLINE items
      | item items
      | ref items
      ;

ref : refid text NEWLINE 
    {
        if ($1 == 0) {
            dpl_parse_error (ctx, "Task ids must not be 0");
            return DPL_ERR_INPUT;
        }
        DPL_FORWARD_ERROR (dpl_parse_entry_new (ctx, ENTRY_TASK));
        DPL_FORWARD_ERROR (dpl_entry_task_id_set (ctx->current, $1));
    }
      itembody 
    {
        DPL_FORWARD_ERROR (dpl_parse_desc_set (ctx));
    }
    
item : time whitespace text NEWLINE 
     {
        DPL_FORWARD_ERROR (dpl_parse_entry_new (ctx, ENTRY_WORK));
     }
       itembody
     {
        DPL_FORWARD_ERROR (dpl_parse_desc_set (ctx));
     }
     | time NEWLINE
     {
        ctx->textpos = 0;
        DPL_FORWARD_ERROR (dpl_parse_entry_new (ctx, ENTRY_WORK));
     }
     | time whitespace NEWLINE
     {
        ctx->textpos = 0;
        DPL_FORWARD_ERROR (dpl_parse_entry_new (ctx, ENTRY_WORK));
     }
     ;

itembody :                               { ctx->textpos = 0; } 
         | INDENT multilinetext DEINDENT { $$ = $2; }
         ;

date : number HYPHEN number HYPHEN number
     {
        /* TODO: check for the isdst flag */
        ctx->tm.tm_sec = 0;
        ctx->tm.tm_min = 0;
        ctx->tm.tm_hour = 0;
        ctx->tm.tm_mday = $5;
        ctx->tm.tm_mon = $3 - 1;
        ctx->tm.tm_year = $1 - 1900;
        ctx->tm.tm_isdst = -1;
     }
     ;

number : DIGIT         { $$ = ctx->c - 48; }
       | number DIGIT  { $$ = $1 * 10 + (ctx->c - 48); }
       ;

whitespace : SPACE
           | TAB
           ;

whitespaces : whitespace
            | whitespaces whitespace
            ;

paddednewline : NEWLINE emptylines
              | whitespaces NEWLINE emptylines
              ;

emptylines : NEWLINE emptylines
           |
           ;

refid : HASH number whitespace { $$ = $2; }
      ;

time : number COLON number 
     { 
        ctx->tm.tm_min = $3;
        ctx->tm.tm_hour = $1;
     }
     ;

multilinetext : multilinetextchar
              {
                  ctx->textpos = 0;
                  DPL_ADD_CHAR(ctx, $1);
              }
              | multilinetext multilinetextchar
              {
                  DPL_ADD_CHAR(ctx, $2);
              }
              ;

multilinetextchar : textchar { $$ = $1; }
                  | NEWLINE  { $$ = '\n'; }
                  ;

textchar  : LETTER { $$ = ctx->c; }
          | DIGIT  { $$ = ctx->c; }
          | COLON  { $$ = ctx->c; }
          | HASH   { $$ = ctx->c; }
          | HYPHEN { $$ = ctx->c; }
          | SPACE  { $$ = ctx->c; }
          | TAB    { $$ = ctx->c; }
          ;


text : textchar
     {
         ctx->textpos = 0;
         DPL_ADD_CHAR(ctx, $1);
     }
     | text textchar
     {
         DPL_ADD_CHAR(ctx, $2);
     } 
     ;
%%


/* Process one of five possible indent changes.
 *
 * Preconditions
 *   - ctx is initialized
 *
 * INDENT, DEINDENT
 *   Preconditions
 *     - ctx->indent1, ctx->indent2 and new_indent are a valid combination of
 *       values
 *   Postconditions
 *     - ctx->indent1 and ctx->indent2 are changed if necessary
 *
 * 0
 *   Preconditions
 *     - ctx->indent1, ctx->indent2 and new_indent are an invalid combination 
 *       of values
 */
static int dpl_lex_change_indent (DplParseContext *ctx, int new_indent)
{
    if (!ctx->indent1) {
        /* 1. noindent -> indent1 */
        ctx->indent1 = new_indent;
        ctx->indent2 = 0;
        return INDENT;
    } else if (ctx->indent1 && new_indent == 0 && ctx->indent2 == 0) {
        /* 2. indent1 -> noindent */
        ctx->indent1 = ctx->indent2 = 0;
        return DEINDENT;
    } else if (ctx->indent1 && (new_indent > ctx->indent1) && ctx->indent2 == 0) {
        /* 3. indent1 -> indent2 */
        ctx->indent2 = new_indent - ctx->indent1;
        return INDENT;
    } else if (ctx->indent2 && new_indent == 0) {
        /* 4. indent2 -> noindent */
        ctx->indent1 = ctx->indent2 = 0;
        ctx->cache_deindent = 1;
        return DEINDENT;
    } else if (ctx->indent2 && (new_indent == ctx->indent1)) {
        /* 5. indent2 -> indent1 */
        ctx->indent2 = 0;
        return DEINDENT;
    } else {
        dpl_parse_error (ctx, "Indendation error");
        return 0;
    }
}


/* Convert a character to a token.
 *
 * Preconditions
 *   - ctx is initialized
 *
 * DEINDENT
 *   Preconditions
 *     - The end of the input file is reached.
 *   Postconditions
 *     - ctx->cache_deindent, ctx->indent1 and ctx->indent2 are updated
 *
 * NEWLINE
 *   Preconditions
 *     - The next character is a newline.
 *   Postconditions
 *     - ctx->lineno and ctx->cache_parseindent are updated
 *
 * DIGIT, TAB, SPACE, HYPHEN, HASH, COLON, LETTER
 *   Preconditions
 *     - The next character is one of the above.
 */
static int dpl_lex_char_to_token (DplParseContext *ctx)
{
    if (ctx->c == EOF) {
        if (ctx->indent2) {
            ungetc (ctx->c, ctx->file);
            ctx->cache_deindent = 1;
            ctx->indent2 = 0;
            return DEINDENT;
        } else if (ctx->indent1) {
            ungetc (ctx->c, ctx->file);
            ctx->indent1 = 0;
            return DEINDENT;
        } else {
            return 0;
        }
    } else if (isdigit (ctx->c)) {
        return DIGIT;
    } else {
        switch (ctx->c) {
            case '\n':
                ctx->cache_parseindent = 1;
                ctx->lineno += 1;
                return NEWLINE;
            case '\t': return TAB;
            case ' ' : return SPACE;
            case '-' : return HYPHEN;
            case '#' : return HASH;
            case ':' : return COLON;
            default:   return LETTER;
        }
    }
}


/* Return the next token.
 *
 * Preconditions
 *   - ctx is initialized
 *   - ctx->file opened
 *
 * INDENT, DEINDENT, NEWLINE, DIGIT, TAB, SPACE, HYPHEN, HASH, COLON, LETTER
 *   Preconditions
 *     - The next token is one of the above.
 * 
 * 0
 *   Preconditions
 *     - No more tokens are available.
 */
static int yylex (DplParseContext *ctx)
{
    /* This works for unicode to, though for multi-byte unicode characterseach 
     * byte is returned as a separate LETTER.
     * This works, as the most significant bit of DIGIT, ' ', '\t' and '\n' is
     * not 1.
     */ 
    ctx->c = fgetc (ctx->file);

    if (ctx->cache_deindent) {
        ctx->cache_deindent = 0;
        return DEINDENT;
    }

    if (ctx->cache_parseindent) {
        int new_indent = 0;

        while (ctx->c == ' ' || ctx->c == '\t') {
            new_indent += 1;

            ctx->c = fgetc (ctx->file);

            /* Ignore whitespaces after the second indendation level is 
             * reached. */
            if (ctx->indent2 && new_indent == (ctx->indent1 + ctx->indent2)) {
                break;
            }
        }

        if (ctx->c == '\n') {
            ctx->lineno += 1;
            return NEWLINE;
        }

        ctx->cache_parseindent = 0;

        if (new_indent != (ctx->indent1 + ctx->indent2)) {
            /* whenever returning an indent or deindent, the last retrieved
             * character is relevant for further tokens. */
            ungetc (ctx->c, ctx->file);

            return dpl_lex_change_indent (ctx, new_indent);
        }
    }

    return dpl_lex_char_to_token (ctx);
}


int dpl_parse_file (const char *filename, DplList **list, int strict)
{
    FILE *f;
    int ret;

    if (!(f = fopen (filename, "r"))) {
        return DPL_ERR_IO;
    }

    ret = dpl_parse (f, filename, list, strict);

    fclose (f);

    return ret;
}


int dpl_parse (FILE *file, const char *filename, DplList **list, int strict)
{
    int ret = DPL_OK;

    DplParseContext ctx;

    /* to initialize the variable */
    tzset ();

#ifdef DEBUG
    yydebug = 1;
#endif
    ctx.strict = strict;
    ctx.file = file;
    ctx.filename = filename;
    ctx.lineno = 1;
    ctx.text = 0;
    ctx.textlen = ctx.textpos = 0;
    ctx.indent1 = ctx.indent2 = 0;
    ctx.cache_deindent = ctx.cache_parseindent = 0;

    DPL_ADD_CHAR(&ctx, 0);
    ctx.textpos = 0;

    if ((ret = dpl_list_new (&ctx.entries)) != DPL_OK) {
        free (ctx.text);
        fclose (ctx.file);
        return ret;
    }

    /* Default yyparse return codes are converted. Other return codes (which
     * must come from dpl functions) are taken as is. */
    switch (ret = yyparse (&ctx)) {
        case 0:  ret = DPL_OK;         break;
        case 1:  ret = DPL_ERR_SYNTAX; break;
        case 2:  ret = DPL_ERR_MEM;    break;
    }

    free (ctx.text);

    *list = ctx.entries;

    if (ret == DPL_OK) {
        DPL_FORWARD_ERROR (dpl_parse_end_set (&ctx));
        DPL_FORWARD_ERROR (dpl_parse_marker_remove (&ctx));
        DPL_FORWARD_ERROR (dpl_parse_link_entries (&ctx));
    }

    return ret;
}
