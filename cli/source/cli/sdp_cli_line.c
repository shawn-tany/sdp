#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "sdp_cli_line.h"

#define LINE_STR_CURSOR(line) ((line)->cur_line.buff + (line)->cursor)
#define LINE_LEN_CURSOR(line) ((line)->cur_line.length - (line)->cursor)

CLI_LINE_T *cli_line_init(int fdout)
{
    CLI_LINE_T *cli_line = NULL;

    cli_line = (CLI_LINE_T *)malloc(sizeof(*cli_line));
    if (!cli_line)
    {
        return NULL;
    }

    memset(cli_line, 0, sizeof(*cli_line));
    cli_line->fdout = fdout;
    sdp_list_init(&cli_line->list);

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

    int length = cli_line->cur_line.length;

    if (length >= ITEM(cli_line->cur_line.buff))
    {
        return 0;
    }

    cli_line->cur_line.buff[length] = ' ';
    
    cli_line->cursor++;
    cli_line->cur_line.length++;

    cli_line_printc(cli_line, ' ');

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

    cli_line_prints(cli_line, LINE_STR_CURSOR(cli_line), LINE_LEN_CURSOR(cli_line));
    cli_line_prints(cli_line, " \b", 2);

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

    if (CLI_LINE_INPUT_TYPE_REPLACE == cli_line->status.input_type)
    {
        cli_line->cur_line.buff[line_cursor] = ch;
        cli_line->cursor++;

        if (cli_line->cursor >= cli_line->cur_line.length)
        {            
            cli_line->cur_line.length++;
        }

        cli_line_printc(cli_line, ch);
    }
    else
    {
        for (i = line_length; i > line_cursor; --i)
        {
            if (i >= (ITEM(cli_line->cur_line.buff)))
            {
                continue;
            }
        
            cli_line->cur_line.buff[i] = cli_line->cur_line.buff[i - 1];
        }
        
        cli_line->cur_line.buff[line_cursor] = ch;
        cli_line->cur_line.length++;    
        cli_line->cursor++;

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

    for (i = 0; i < cli_line->cursor; ++i)
    {
        cli_line_printc(cli_line, 0x1b);
        cli_line_printc(cli_line, 0x5b);
        cli_line_printc(cli_line, 0x44);
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

    char ch = '\n';

    cli_line_his_add(cli_line);

    cli_line_prints(cli_line, &ch, 1);

    cli_line_reset(cli_line);

    cli_line_print_line(linehead, cli_line);

    cli_line_cursor_reset(cli_line);

    return 0;
}

int cli_line_his_add(CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    int his_index = 0;
    SDP_CLI_LINE_CONTENT_T *new_his = NULL;

    if (!cli_line->cur_line.length)
    {
        return 0;
    }

    if ((cli_line->his_pos && !strcmp(cli_line->his_pos->buff, cli_line->cur_line.buff)) &&
        cli_line->his_pos->length == cli_line->cur_line.length)
    {
        return 0;
    }

    for (his_index = 0; his_index < ITEM(cli_line->his_line); ++his_index)
    {
        if (!cli_line->his_line[his_index].length)
        {
            break;
        }
    }

    if (his_index >= ITEM(cli_line->his_line))
    {
        new_his = sdp_list_entry(cli_line->list.next, SDP_CLI_LINE_CONTENT_T, head);
        if (new_his)
        {
            sdp_list_del(&(new_his->head));
        }
    }
    else
    {
        new_his = &(cli_line->his_line[his_index]);
    }

    memcpy(new_his, &(cli_line->cur_line), sizeof(*new_his));

    sdp_list_add_tail(&(cli_line->list), &(new_his->head));

    cli_line->his_pos  = new_his;
    cli_line->his_last = new_his;

    return 0;
}

int cli_line_his_prev(char *linehead, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(linehead);
    PTR_CHECK_N1(cli_line);

    if (!cli_line->his_pos || !cli_line->his_pos->length)
    {
        return 0;
    }

    cli_line_clean(cli_line);

    cli_line_prints(cli_line, cli_line->his_pos->buff, cli_line->his_pos->length);

    cli_line->cursor = cli_line->his_pos->length;

    cli_line->his_pos = sdp_list_entry(cli_line->his_pos->head.prev, SDP_CLI_LINE_CONTENT_T, head);

    return 0;
}

int cli_line_his_next(char *linehead, CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(linehead);
    PTR_CHECK_N1(cli_line);

    if (!cli_line->his_pos)
    {
        return 0;
    }
    
    cli_line_clean(cli_line);

    cli_line->his_pos = sdp_list_entry(cli_line->his_pos->head.next, SDP_CLI_LINE_CONTENT_T, head);

    if (!cli_line->his_pos || !cli_line->his_pos->length)
    {
        return 0;
    }

    cli_line_prints(cli_line, cli_line->his_pos->buff, cli_line->his_pos->length);

    cli_line->cursor = cli_line->his_pos->length;

    return 0;
}


