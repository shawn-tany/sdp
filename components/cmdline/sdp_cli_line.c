#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "sdp_cli_line.h"

#define LINE_STR_CURSOR(line) ((line)->cur_line.buff + (line)->cursor)
#define LINE_LEN_CURSOR(line) ((line)->cur_line.length - (line)->cursor)

static int cli_line_content_buff_end(CLI_LINE_CONTENT_T *content)
{
    PTR_CHECK_N1(content);

    int buff_end = 0;

    buff_end = ((content->length >= ITEM(content->buff)) ? (ITEM(content->buff) - 1) : content->length);

    content->buff[buff_end] = 0;

    return 0;
}

CLI_LINE_T *cli_line_init(int fdout)
{
    int i = 0;
    CLI_LINE_T *cli_line = NULL;

    cli_line = (CLI_LINE_T *)malloc(sizeof(*cli_line));
    if (!cli_line)
    {
        return NULL;
    }

    /* init cli line info */
    memset(cli_line, 0, sizeof(*cli_line));
    cli_line->fdout = fdout;

    /* init history line */
    INIT_LIST_HEAD(&cli_line->history.list);
    for (i = 0; i < ITEM(cli_line->history.line); ++i)
    {
        list_add_tail(&cli_line->history.list, &cli_line->history.line[i].head);
    }

    cli_line->history.pos = &cli_line->history.line[0];
    cli_line->history.last = &cli_line->history.line[0];

    return cli_line;
}

int cli_line_uninit(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    free(cli_line);

    return 0;
}

/* input */
int cli_line_printc(CLI_LINE_T *cli_line, char ch)
{
    PTR_CHECK_N1(cli_line);

    return write(cli_line->fdout, &ch, 1);
}

int cli_line_prints(CLI_LINE_T *cli_line, char *string, int length)
{
    PTR_CHECK_N1(cli_line);
    PTR_CHECK_N1(string);

    if (0 >= length)
    {
        return 0;
    }

    return write(cli_line->fdout, string, length);
}

int cli_line_separate(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    /* insert a whitespace */
    cli_line_insert(' ', cli_line);

    return 0;
}

int cli_line_enter(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    /* insert a whitespace */
    cli_line_printc(cli_line, '\n');

    return 0;
}

int cli_line_tab(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    /* insert a whitespace */
    cli_line_printc(cli_line, '\t');

    return 0;
}

int cli_line_backspace(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    int i = 0;

    if (0 >= cli_line->cursor)
    {
        return 0;
    }

    /* updata cursor */
    cli_line->cursor--;

    /* updata line buff */
    for (i = cli_line->cursor; i < cli_line->cur_line.length; ++i)
    {
        if (i >= (ITEM(cli_line->cur_line.buff) - 1))
        {
            break;
        }
    
        cli_line->cur_line.buff[i] = cli_line->cur_line.buff[i + 1];
    }
    cli_line->cur_line.length--;
    cli_line_content_buff_end(&cli_line->cur_line);

    /* updata display line */
    cli_line_printc(cli_line, '\b');
    cli_line_prints(cli_line, LINE_STR_CURSOR(cli_line), LINE_LEN_CURSOR(cli_line));
    cli_line_prints(cli_line, " \b", 2);

    /* recovery cursor */
    for (i = 0; i < LINE_LEN_CURSOR(cli_line); ++i)
    {
        cli_line_printc(cli_line, '\b');
    }

    return 0;
}

int cli_line_delete(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    int i = 0;

    if (cli_line->cursor >= cli_line->cur_line.length)
    {
        return 0;
    }

    /* updata line buff */
    for (i = cli_line->cursor; i < cli_line->cur_line.length; ++i)
    {
        if (i >= (ITEM(cli_line->cur_line.buff) - 1))
        {
            break;
        }
    
        cli_line->cur_line.buff[i] = cli_line->cur_line.buff[i + 1];
    }
    cli_line->cur_line.length--;
    cli_line_content_buff_end(&cli_line->cur_line);

    /* updata display line */
    cli_line_prints(cli_line, LINE_STR_CURSOR(cli_line), LINE_LEN_CURSOR(cli_line));
    cli_line_prints(cli_line, " \b", 2);

    /* recovery cursor */
    for (i = 0; i < LINE_LEN_CURSOR(cli_line); ++i)
    {
        cli_line_printc(cli_line, 0x1b);
        cli_line_printc(cli_line, 0x5b);
        cli_line_printc(cli_line, 0x44);
    }

    return 0;
}

int cli_line_insert(char ch, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    int i = 0;
    int line_length  = cli_line->cur_line.length;
    int line_cursor  = cli_line->cursor;

    if (line_cursor >= ITEM(cli_line->cur_line.buff))
    {
        return 0;
    }

    /* replace */
    if (CLI_LINE_INPUT_TYPE_REPLACE == cli_line->status.input_type)
    {
        /* updata line buff */
        cli_line->cur_line.buff[line_cursor] = ch;

        /* updata cursor */
        cli_line->cursor++;

        if (cli_line->cursor >= cli_line->cur_line.length)
        {            
            cli_line->cur_line.length++;
            cli_line_content_buff_end(&cli_line->cur_line);
        }

        /* updata display line */
        cli_line_printc(cli_line, ch);
    }
    /* add */
    else
    {
        /* move line characters forward */
        for (i = line_length; i > line_cursor; --i)
        {
            if (i >= (ITEM(cli_line->cur_line.buff)))
            {
                continue;
            }
        
            cli_line->cur_line.buff[i] = cli_line->cur_line.buff[i - 1];
        }
        
        /* insert a character */
        cli_line->cur_line.buff[line_cursor] = ch;
        cli_line->cur_line.length++;    
        cli_line_content_buff_end(&cli_line->cur_line);
        
        /* updata cursor */
        cli_line->cursor++;

        /* updata display line */
        cli_line_printc(cli_line, ch);
        cli_line_prints(cli_line, LINE_STR_CURSOR(cli_line), LINE_LEN_CURSOR(cli_line));

        /* recovery cursor */
        for (i = 0; i < LINE_LEN_CURSOR(cli_line); ++i)
        {
            cli_line_printc(cli_line, 0x1b);
            cli_line_printc(cli_line, 0x5b);
            cli_line_printc(cli_line, 0x44);
        }
    }
    
    return 0;
}

int cli_line_replace(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    int is_replace = (CLI_LINE_INPUT_TYPE_REPLACE == cli_line->status.input_type);

    /* updata input type */
    cli_line->status.input_type = is_replace ? CLI_LINE_INPUT_TYPE_ADD : CLI_LINE_INPUT_TYPE_REPLACE;
    
    return 0;
}

/* cursor */
int cli_line_cursor_reset(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line); 

    cli_line->cursor = 0;

    return 0;
}

int cli_line_cursor_left(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);    

    if (0 >= cli_line->cursor)
    {
        return 0;
    }
    
    cli_line_printc(cli_line, 0x1b);
    cli_line_printc(cli_line, 0x5b);
    cli_line_printc(cli_line, 0x44);

    cli_line->cursor -= 1;    

    return 0;
}

int cli_line_cursor_right(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);    

    if (cli_line->cursor >= cli_line->cur_line.length)
    {
        return 0;
    }

    cli_line_printc(cli_line, 0x1b);
    cli_line_printc(cli_line, 0x5b);
    cli_line_printc(cli_line, 0x43);

    cli_line->cursor += 1;

    return 0;
}

/* line */
int cli_line_reset(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line); 

    cli_line->cur_line.buff[0] = 0;
    cli_line->cur_line.length = 0;

    return 0;
}

int cli_line_clean(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    int i = 0;
    int cursor = cli_line->cursor;
    int length = cli_line->cur_line.length;

    for (i = 0; i < cursor; ++i)
    {
        cli_line_backspace(cli_line);
    }

    for (; i < length; ++i)
    {
        cli_line_delete(cli_line);
    }

    return 0;
}

int cli_line_print_line(char *linehead, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(linehead);
    PTR_CHECK_N1(cli_line);

    if (linehead)
    {
        cli_line_prints(cli_line, linehead, strlen(linehead));
    }

    if (cli_line->cur_line.length)
    {
        cli_line_prints(cli_line, cli_line->cur_line.buff, cli_line->cur_line.length);
    }

    return 0;
}

int cli_line_new(char *linehead, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(linehead);
    PTR_CHECK_N1(cli_line);

    /* show enter */
    cli_line_enter(cli_line);
    
    /* add history line */
    cli_line_his_add(cli_line);

    /* reset history line pos */
    cli_line_his_reset(cli_line);

    /* reset line buff */
    cli_line_reset(cli_line);

    /* show line */
    cli_line_print_line(linehead, cli_line);

    /* reset line cursor */
    cli_line_cursor_reset(cli_line);

    return 0;
}

int cli_line_his_add(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    CLI_LINE_CONTENT_T *new = NULL;
    CLI_LINE_CONTENT_T *new_pos = NULL;
    CLI_LINE_CONTENT_T *last = NULL;

    if (!cli_line->cur_line.length)
    {
        return 0;
    }

    last = cli_line->history.last;

    /* same history line */
    if (!strcmp(last->buff, cli_line->cur_line.buff) &&
        last->length == cli_line->cur_line.length)
    {
        return 0;
    }

    /* history empty */
    if (!cli_line->history.number)
    {
        new = cli_line->history.last;
    }
    /* histroy full */
    else if (ITEM(cli_line->history.line) <= cli_line->history.number)
    {
        /* get first */
        new = list_first_entry(&cli_line->history.list, CLI_LINE_CONTENT_T, head);
        
        /* delete first */
        list_del(&new->head);

        /* add new history */
        list_add_tail(&cli_line->history.list, &new->head);

        cli_line->history.number--;
    }
    else
    {
        new = list_next_entry(last, head);
    }

    cli_line->history.last = new;

    memcpy(new->buff, cli_line->cur_line.buff, sizeof(new->buff));
    new->length = cli_line->cur_line.length;

    new_pos = list_next_entry(last, head);
    cli_line->history.pos = new_pos;

    /* updata history number & cursor */
    cli_line->history.number++;
    cli_line->history.cursor = cli_line->history.number + 1;

    return 0;
}

int cli_line_his_prev(char *linehead, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(linehead);
    PTR_CHECK_N1(cli_line);

    int i = 0;
    CLI_LINE_CONTENT_T *show = NULL;

    /* empty */
    if (!cli_line->history.number)
    {
        return 0;
    }

    /* is first */
    if (1 >= cli_line->history.cursor)
    {
        return 0;
    }

    /* will last */
    if (cli_line->history.number < cli_line->history.cursor)
    {
        /* record cache line */
        memcpy(cli_line->cache_line.buff, cli_line->cur_line.buff, sizeof(ITEM(cli_line->cache_line.buff)));
        cli_line->cache_line.length = cli_line->cur_line.length;

        cli_line->history.pos = cli_line->history.last;
    }
    else
    {
        cli_line->history.pos = list_prev_entry(cli_line->history.pos, head);
    }

    cli_line->history.cursor--;

    show = cli_line->history.pos;

    /* clean display and buff, should be execute after save current to cache */
    cli_line_clean(cli_line);

    for (i = 0; i < show->length; ++i)
    {
        cli_line_insert(show->buff[i], cli_line);
    }

    return 0;
}

int cli_line_his_next(char *linehead, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(linehead);
    PTR_CHECK_N1(cli_line);

    int i = 0;
    CLI_LINE_CONTENT_T *show = NULL;

    /* empty */
    if (!cli_line->history.number)
    {
        return 0;
    }

    /* is cache */
    if (cli_line->history.number < cli_line->history.cursor)
    {
        return 0;
    }

    cli_line->history.cursor++;

    /* is last */
    if ((cli_line->history.number + 1) == cli_line->history.cursor)
    {
        show = &cli_line->cache_line;
    }
    else
    {
        cli_line->history.pos = list_next_entry(cli_line->history.pos, head);
        show = cli_line->history.pos;
    }

    /* clean display and buff, should be execute after save current to cache */
    cli_line_clean(cli_line);

    for (i = 0; i < show->length; ++i)
    {
        cli_line_insert(show->buff[i], cli_line);
    }

    return 0;
}

int cli_line_his_reset(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);
    
    cli_line->history.pos = list_next_entry(cli_line->history.last, head);

    /* updata history number & cursor */
    cli_line->history.cursor = cli_line->history.number + 1;

    return 0;
}
