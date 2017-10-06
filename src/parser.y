%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "dpl/parser.h"
#include "dpl/defs.h"
#include "dpl/utils.h"

static DplTaskList *tasks;
static DplTask *current;
static time_t daytime;

struct _Reference {
    int id;
    DplRef *ref;
    struct _Reference *next;
};

static struct _Reference *refs;

static int indent, indent2; /* maximally 2 indent levels are needed. */
static int yyc;
static char *yytext;
static size_t yytextbufferlen;
static size_t yytextpos;

static char *yymultitext;
static size_t yymultitextbufferlen;
static size_t yymultitextpos;

static FILE *yyin;
static const char *yyfilename;
static size_t yylineno;

static int yystrict;

#define RTRIM_WHITESPACE(s, len) { \
    while (s && len > 0 && s[len-1] == ' ') { \
        s[len-1] = 0; \
        len -= 1; \
    } \
}


static int dpl_parse_end_set (DplTaskList *tasks)
{
    DplTaskListIter *iter;
    DplTaskListFilter *taskfilter;
    DplTask *curr, *next;
    const char *title;

    DPL_FORWARD_ERROR (dpl_tasklist_iter (tasks, &iter));
    if (dpl_tasklistiter_next (iter, &curr) != DPL_OK) {
        /* empty task list */
        return DPL_OK;
    }

    while (dpl_tasklistiter_next (iter, &next) == DPL_OK) {
        time_t begin;
        DplTaskType type;
        DPL_FORWARD_ERROR (dpl_task_type_get (curr, &type));
        if (type == task_type) {
            DPL_FORWARD_ERROR (dpl_task_begin_get (next, &begin));
        } else {
            DPL_FORWARD_ERROR (dpl_task_begin_get (curr, &begin));
        }
        DPL_FORWARD_ERROR (dpl_task_end_set (curr, begin));

        curr = next;
    }

    /* curr is the last task and needs to be an end marker */
    DPL_FORWARD_ERROR (dpl_task_title_get (curr, &title));
    if (title) {
        yyerror ("No end marker task found");
        return DPL_ERR_INPUT;
    }

    DPL_FORWARD_ERROR (dpl_tasklistiter_free (iter));
    DPL_FORWARD_ERROR (dpl_tasklist_iter (tasks, &iter));

    while (dpl_tasklistiter_next (iter, &curr) == DPL_OK) {
        const char *title;
        DPL_FORWARD_ERROR (dpl_task_title_get (curr, &title));

        if (!title) {
            DPL_FORWARD_ERROR (dpl_tasklist_remove (tasks, curr));
        }
    }
    return DPL_OK;
}


static int dpl_parse_copy_open_refs ()
{
    struct _Reference *r = refs;

    while (r) {
        int done;
        const char *title;

        DPL_FORWARD_ERROR (dpl_ref_done_get (r->ref, &done));
        if (!done) {
            DplTask *task;

            DPL_FORWARD_ERROR (dpl_ref_title_get (r->ref, &title));
            DPL_FORWARD_ERROR (dpl_task_new (&task));
            DPL_FORWARD_ERROR (dpl_task_begin_set (task, daytime));
            DPL_FORWARD_ERROR (dpl_task_title_set (task, title));
            DPL_FORWARD_ERROR (dpl_task_ref_set (task, r->ref));
            DPL_FORWARD_ERROR (dpl_task_type_set (task, ref_type));
            DPL_FORWARD_ERROR (dpl_tasklist_push (tasks, task));
        }
        r = r->next;
    }

    return DPL_OK;
}


static int dpl_parse_get_reference (int id, DplRef **ref) 
{
    struct _Reference *r = refs;

    while (r) {
        if (r->id == id) {
            *ref = r->ref;
            return DPL_OK;
        }
        r = r->next;
    }

    return DPL_ERR_NOTFOUND;
}


static int dpl_parse_add_reference (int id, const DplRef *ref)
{
    struct _Reference *newref = malloc (sizeof (struct _Reference));

    if (!newref) {
        return DPL_ERR_MEM;
    }

    struct _Reference *existing_ref;
    if (dpl_parse_get_reference (id, &existing_ref) == DPL_OK) {
        char errmsg[1024];
        snprintf (errmsg, 1024, "Duplicate task id #%d", id);
        DPL_FORWARD_ERROR (yywarning (errmsg));
    }

    newref->ref = ref;
    newref->next = refs;
    newref->id = id;

    refs = newref;

    return DPL_OK;
}


static int dpl_parse_free_references ()
{
    while (refs) {
        struct _Reference *next = refs->next;
        free (refs);
        refs = next;
    }
    return DPL_OK;
}


static int dpl_parse_new_task (time_t begin, DplTask **task, int newref)
{
    DplRef *ref = 0;
    const char *title, *desc;

    RTRIM_WHITESPACE (yytext, yytextpos);
    RTRIM_WHITESPACE (yymultitext, yymultitextpos);

    title = yytextpos ? dpl_skip_whitespaces (yytext) : 0;
    desc = yymultitextpos ? dpl_skip_whitespaces (yymultitext) : 0;

    if (newref) {
        DPL_FORWARD_ERROR (dpl_ref_new (&ref));
        DPL_FORWARD_ERROR (dpl_ref_title_set (ref, title));
        DPL_FORWARD_ERROR (dpl_ref_desc_set (ref, desc));
        title = desc = 0;
    } else if (!newref && title && (title[0] == '#' || title[0] == '+')) {
        int id = -1;
        int done = 0;
        if (sscanf (title, "+#%d", &id)) {
            done = 1;
        } else {
            sscanf (title, "#%d", &id);
        }
        if (id >= 0) {
            if (dpl_parse_get_reference (id, &ref) != DPL_OK) {
                ref = 0;
            } else if (done) {
                DPL_FORWARD_ERROR (dpl_ref_done_set (ref, done));
            }
        }
    }

    DPL_FORWARD_ERROR (dpl_task_new (task));
    DPL_FORWARD_ERROR (dpl_task_begin_set (*task, daytime + begin));
    DPL_FORWARD_ERROR (dpl_task_title_set (*task, title));
    DPL_FORWARD_ERROR (dpl_task_desc_set (*task, desc));
    if (ref) {
        DPL_FORWARD_ERROR (dpl_task_ref_set (*task, ref));
    }
    if (newref) {
        DPL_FORWARD_ERROR (dpl_task_type_set (*task, ref_type));
    }
    
    /* reset buffers */
    yytextpos = 0;
    yymultitextpos = 0;
    yytext[0] = 0;
    yymultitext[0] = 0;

    return DPL_OK;
}

%}

%start days
%token DIGIT LETTER COLON HYPHEN NEWLINE SPACE TAB HASH INDENT DEINDENT

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
      | item items
      | ref items
      ;

ref : refid text NEWLINE itembody 
    {
        DplRef *ref;
        DPL_FORWARD_ERROR (dpl_parse_new_task (0, &current, 1));
        DPL_FORWARD_ERROR (dpl_task_ref_get (current, &ref));
        DPL_FORWARD_ERROR (dpl_ref_id_set (ref, $1));
        DPL_FORWARD_ERROR (dpl_parse_add_reference ($1, ref));
        DPL_FORWARD_ERROR (dpl_tasklist_push (tasks, current));
    }
    
item : time whitespace text NEWLINE itembody
     {
        DPL_FORWARD_ERROR (dpl_parse_new_task ($1, &current, 0));
        DPL_FORWARD_ERROR (dpl_tasklist_push (tasks, current));
     }
     | time NEWLINE
     {
        DPL_FORWARD_ERROR (dpl_parse_new_task ($1, &current, 0));
        DPL_FORWARD_ERROR (dpl_tasklist_push (tasks, current));
     }
     | time whitespace NEWLINE
     {
        DPL_FORWARD_ERROR (dpl_parse_new_task ($1, &current, 0));
        DPL_FORWARD_ERROR (dpl_tasklist_push (tasks, current));
     }
     ;

itembody :                               { $$ = 0; } 
         | INDENT multilinetext DEINDENT { $$ = $2; }
         ;

date : number HYPHEN number HYPHEN number
     {
        struct tm date = { 0, 0, 0, $5, $3 - 1, $1 - 1900, 0, 0, 1 };
        daytime = mktime (&date);
        dpl_parse_copy_open_refs ();
     }
     ;

number : DIGIT         { $$ = yyc - 48; }
       | number DIGIT  { $$ = $1 * 10 + (yyc - 48); }
       ;

whitespace : SPACE
           | TAB
           ;

whitespaces : whitespace
            | whitespaces whitespace
            ;

paddednewline : NEWLINE
              | whitespaces NEWLINE
              ;

refid : HASH number whitespace { $$ = $2; }
      ;

time : number COLON number { $$ = ($1 * 3600) + ($3 * 60); }
     ;

multilinetext : multilinetextchar
              {
                  yymultitext[0] = $1;
                  yymultitext[1] = 0;
                  yymultitextpos = 1;
                  $$ = yymultitext;
              }
              | multilinetext multilinetextchar
              {
                  if (yymultitextpos >= yymultitextbufferlen) {
                      yymultitextbufferlen *= 2;
                      yymultitext = realloc (yymultitext, yymultitextbufferlen);
                      if (!yymultitext) {
                          return DPL_ERR_MEM;
                      }
                  }
                  yymultitext[yymultitextpos] = $2;
                  yymultitext[yymultitextpos + 1] = 0;
                  yymultitextpos += 1;
                  $$ = yymultitext;
              }
              ;

multilinetextchar : textchar { $$ = $1; }
                  | NEWLINE  { $$ = '\n'; }
                  ;

textchar  : LETTER { $$ = yyc; }
          | DIGIT  { $$ = yyc; }
          | COLON  { $$ = yyc; }
          | HASH   { $$ = yyc; }
          | HYPHEN { $$ = yyc; }
          | SPACE  { $$ = yyc; }
          | TAB    { $$ = yyc; }
          ;


text : textchar
     {
         yytext[0] = $1;
         yytext[1] = 0;
         yytextpos = 1;
         $$ = yytext;
     }
     | text textchar
     {
        if (yytextpos >= yytextbufferlen) {
            yytextbufferlen *= 2;
            yytext = realloc (yytext, yytextbufferlen);
            if (!yytext) {
                return DPL_ERR_MEM;
            }
        }
        yytext[yytextpos] = $2;
        yytext[yytextpos + 1] = 0;
        yytextpos += 1;
        $$ = yytext;
     } 
     ;
%%
int yylex ()
{
    /* This works for unicode to, though for multi-byte unicode characterseach 
     * byte is returned as a separate LETTER.
     * This works, as the most significant bit of DIGIT, ' ', '\t' and '\n' is
     * not 1.
     */ 
    yyc = fgetc (yyin);
    static int parse_indent = 0;
    static int cache_deindent = 0;

    /*
        1. No indent - Date
        2. Indentlevel 1 - Task/Ref header
        3. Indentlevel 2 - Description

        1. noindent -> noindent
        6. indent1 -> indent1
        9. indent2 -> indent2


        2. noindent -> indent1
        4. indent1 -> noindent
        5. indent1 -> indent2
        7. indent2 -> noindent
        8. indent2 -> indent1
     */

    if (cache_deindent) {
        cache_deindent = 0;
        return DEINDENT;
    }

    if (parse_indent) {
        int new_indent = 0;

        while (yyc == ' ' || yyc == '\t') {
            new_indent += 1;
            yyc = fgetc (yyin);
        }

        if (yyc == '\n') {
            /* ignore empty lines. */
            yylineno += 1;
            return NEWLINE;
            //return yylex ();
        }

        parse_indent = 0;

        if (new_indent != (indent + indent2)) {
            ungetc (yyc, yyin);

            if (!indent) {
                /* noindent -> indent1 */
                indent = new_indent;
                indent2 = 0;
                return INDENT;
            } else if (indent && new_indent == 0 && indent2 == 0) {
                /* indent1 -> noindent */
                indent = indent2 = 0;
                return DEINDENT;
            } else if (indent && (new_indent > indent) && indent2 == 0) {
                /* indent1 -> indent2 */
                indent2 = new_indent - indent;
                return INDENT;
            } else if (indent2 && new_indent == 0) {
                /* indent2 -> noindent */
                indent = indent2 = 0;
                cache_deindent = 1;
                return DEINDENT;
            } else if (indent2 && (new_indent == indent)) {
                /* indent2 -> indent1 */
                indent2 = 0;
                return DEINDENT;
            } else {
                yyerror ("Indendation error");
                return 0;
            }
        }
    }

    if (yyc == EOF) {
        if (indent2) {
            ungetc (yyc, yyin);
            cache_deindent;
            indent2 = 0;
            return DEINDENT;
        } else if (indent) {
            ungetc (yyc, yyin);
            indent = 0;
            return DEINDENT;
        } else {
            return 0;
        }
    } else if (isdigit (yyc)) {
        return DIGIT;
    } else if (yyc == '\n') {
        parse_indent = 1;
        yylineno += 1;
        return NEWLINE;
    } else if (yyc == '\t') {
        return TAB;
    } else if (yyc == ' ') {
        return SPACE;
    } else if (yyc == '-') {
        return HYPHEN;
    } else if (yyc == '#') {
        return HASH;
    } else if (yyc == ':') {
        return COLON;
    } else {
        return LETTER;
    }
}


int yyerror (char *s)
{
    fprintf (stderr, "%s:%ld: error: %s\n", yyfilename, yylineno, s);

    return DPL_ERR_SYNTAX;
}


int yywarning (char *s)
{
    if (yystrict) {
        return yyerror (s);
    } else {
        fprintf (stderr, "%s:%ld: warning: %s\n", yyfilename, yylineno, s);
    }

    return DPL_OK;
}


int dpl_parse (const char *filename, DplTaskList **list, int strict)
{
    int ret = DPL_OK;

#ifdef DEBUG
    yydebug = 1;
#endif
    yystrict = strict;
    yyfilename = filename;
    yylineno = 1;
    yytextbufferlen = 1024;
    yytext = malloc (yytextbufferlen);
    yymultitextbufferlen = 1024;
    yymultitext = malloc (yymultitextbufferlen);
    indent = indent2 = 0;

    /* reset buffers */
    yytextpos = 0;
    yymultitextpos = 0;
    yytext[0] = 0;
    yymultitext[0] = 0;

    if (!yytext || !yymultitext) {
        return DPL_ERR_MEM;
    }

    if (!(yyin = fopen (yyfilename, "r"))) {
        return DPL_ERR_IO;
    }

    if ((ret = dpl_tasklist_new (&tasks)) != DPL_OK) {
        fclose (yyin);
        return ret;
    }

    /* FIXME: clear up different return types of yyparse */
    switch (ret = yyparse ()) {
        case 0: ret = DPL_OK;
                break;
        case 1: ret = DPL_ERR_SYNTAX;
                break;
        case 2: ret = DPL_ERR_MEM;
                break;
    }

    *list = tasks;

    fclose (yyin);
    free (yytext);

    /* to pass on the first available error message and avoid futile operations
     */
    if (ret == DPL_OK) {
        DPL_FORWARD_ERROR (dpl_parse_end_set (tasks));
    }
    DPL_FORWARD_ERROR (dpl_parse_free_references ());

    return ret;
}
