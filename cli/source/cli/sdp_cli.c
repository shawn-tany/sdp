#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>

#include "sdp_cli.h"

#define LINE_STR_CURSOR(line) ((line)->his_line.buff + (line)->cursor)
#define LINE_LEN_CURSOR(line) ((line)->his_line.length - (line)->cursor)

static int cli_cursor_move(SDP_CLI_T *sdp_cli, int direction)
{
    PTR_CHECK_N1(sdp_cli);    

    int cursor_inc = 0;
    char ch = 0;

    /* left */
    if (direction)
    {
        ch = 0x44;

        if (0 >= sdp_cli->line.cursor)
        {
            return 0;
        }

        cursor_inc = -1;
    }
    else
    {
        ch = 0x43;

        if (sdp_cli->line.cursor >= sdp_cli->line.his_line.length)
        {
            return 0;
        }

        cursor_inc = 1;
    }

    cli_printc(&sdp_cli->line, 0x1b);
    cli_printc(&sdp_cli->line, 0x5b);
    cli_printc(&sdp_cli->line, ch);

    sdp_cli->line.cursor += cursor_inc;

    return 0;
}

static int cli_separate(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int length = sdp_cli->line.his_line.length;

    if (length >= ITEM(sdp_cli->line.his_line.buff))
    {
        return 0;
    }

    sdp_cli->line.his_line.buff[length] = ' ';
    
    sdp_cli->line.cursor++;
    sdp_cli->line.his_line.length++;

    cli_printc(&sdp_cli->line, ' ');

    sdp_cli->params.param_index++;

    return 0;
}

static int cli_help(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_prints(&sdp_cli->line, sdp_cli->config.prompthead, strlen(sdp_cli->config.prompthead));

    return 0;
}

static int cli_general(char character, SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int i = 0;
    int line_length  = sdp_cli->line.his_line.length;
    int line_cursor  = sdp_cli->line.cursor;

    if (line_cursor >= ITEM(sdp_cli->line.his_line.buff))
    {
        return 0;
    }

    if (SDP_CLI_INPUT_TYPE_REPLACE == sdp_cli->status.input_type)
    {
        sdp_cli->line.his_line.buff[line_cursor] = character;
        sdp_cli->line.cursor++;

        if (sdp_cli->line.cursor >= sdp_cli->line.his_line.length)
        {            
            sdp_cli->line.his_line.length++;
        }

        cli_printc(&sdp_cli->line, character);
    }
    else
    {
        for (i = line_length; i > line_cursor; --i)
        {
            if (i >= (ITEM(sdp_cli->line.his_line.buff)))
            {
                continue;
            }
        
            sdp_cli->line.his_line.buff[i] = sdp_cli->line.his_line.buff[i - 1];
        }
        
        sdp_cli->line.his_line.buff[line_cursor] = character;
        sdp_cli->line.his_line.length++;    
        sdp_cli->line.cursor++;

        cli_printc(&sdp_cli->line, character);
        cli_prints(&sdp_cli->line, LINE_STR_CURSOR(&sdp_cli->line), LINE_LEN_CURSOR(&sdp_cli->line));

        /* recovery cursor */
        for (i = 0; i < LINE_LEN_CURSOR(&sdp_cli->line); ++i)
        {
            cli_printc(&sdp_cli->line, 0x1b);
            cli_printc(&sdp_cli->line, 0x5b);
            cli_printc(&sdp_cli->line, 0x44);
        }
    }
    
    return 0;
}

static int cli_backspace(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int i = 0;

    if (0 >= sdp_cli->line.cursor)
    {
        return 0;
    }

    /* updata cursor */
    sdp_cli->line.cursor--;

    /* updata line buff */
    for (i = sdp_cli->line.cursor; i < sdp_cli->line.his_line.length; ++i)
    {
        if (i >= (ITEM(sdp_cli->line.his_line.buff) - 1))
        {
            break;
        }
    
        sdp_cli->line.his_line.buff[i] = sdp_cli->line.his_line.buff[i + 1];
    }
    sdp_cli->line.his_line.length--;

    /* updata display line */
    cli_printc(&sdp_cli->line, '\b');
    cli_prints(&sdp_cli->line, LINE_STR_CURSOR(&sdp_cli->line), LINE_LEN_CURSOR(&sdp_cli->line));
    cli_prints(&sdp_cli->line, " \b", 2);

    /* recovery cursor */
    for (i = 0; i < LINE_LEN_CURSOR(&sdp_cli->line); ++i)
    {
        cli_printc(&sdp_cli->line, '\b');
    }

    return 0;
}

static int cli_delete(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int i = 0;

    if (sdp_cli->line.cursor >= sdp_cli->line.his_line.length)
    {
        return 0;
    }

    /* updata line buff */
    for (i = sdp_cli->line.cursor; i < sdp_cli->line.his_line.length; ++i)
    {
        if (i >= (ITEM(sdp_cli->line.his_line.buff) - 1))
        {
            break;
        }
    
        sdp_cli->line.his_line.buff[i] = sdp_cli->line.his_line.buff[i + 1];
    }

    sdp_cli->line.his_line.length--;

    cli_prints(&sdp_cli->line, LINE_STR_CURSOR(&sdp_cli->line), LINE_LEN_CURSOR(&sdp_cli->line));
    cli_prints(&sdp_cli->line, " \b", 2);

    for (i = 0; i < LINE_LEN_CURSOR(&sdp_cli->line); ++i)
    {
        cli_printc(&sdp_cli->line, 0x1b);
        cli_printc(&sdp_cli->line, 0x5b);
        cli_printc(&sdp_cli->line, 0x44);
    }

    return 0;
}

static int cli_insert(char character, SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int is_replace = (SDP_CLI_INPUT_TYPE_REPLACE == sdp_cli->status.input_type);

    sdp_cli->status.input_type = is_replace ? SDP_CLI_INPUT_TYPE_ADD : SDP_CLI_INPUT_TYPE_REPLACE;
    
    return 0;
}

static int cli_line_switch(SDP_CLI_T *sdp_cli, int direction)
{
    PTR_CHECK_N1(sdp_cli);    

    if (!direction)
    {
        
    }
    
    return 0;
}

/* input option */
static int cli_execute(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_enter_line(sdp_cli->config.rowhead, &sdp_cli->line);

    if (sdp_cli_cmd_execute(&sdp_cli->cmds))
    {
        
    }
    
    return 0;
}

static int cli_completion(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int i = 0;
    char buff[SDP_CLI_MAX_LINE_LEN + 36] = {0};

    sdp_cli_cmd_complete(&sdp_cli->cmds);

    if (!sdp_cli->cmds.complete.num)
    {
        snprintf(buff, sizeof(buff), "\n\tERROR : No such command : %s", sdp_cli->line.his_line.buff);
        cli_prints(&sdp_cli->line, buff, strlen(buff));
        return -1;
    }
    else if (1 == sdp_cli->cmds.complete.num)
    {
        if (sdp_cli->cmds.complete.separate)
        {
            cli_separate(sdp_cli);
        }
    
        for (i = 0; i < strlen(sdp_cli->cmds.complete.buff[0]); ++i)
        {
            cli_general(sdp_cli->cmds.complete.buff[0][i], sdp_cli);
        }
    }
    else
    {
        cli_printc(&sdp_cli->line, '\n');
    
        for (i = 0; i < sdp_cli->cmds.complete.num; ++i)
        {
            snprintf(buff, sizeof(buff), "\t %s", sdp_cli->cmds.complete.buff[i]);
            cli_prints(&sdp_cli->line, buff, strlen(buff));
        }
        
        cli_enter_line(sdp_cli->config.rowhead, &sdp_cli->line);
    }

    return 0;
}

static int cli_input_char(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    char ch = 0;
    int ret = 0;

    ret = sdp_cli_ch_input(&sdp_cli->chset, &ch);
    if (0 > ret)
    {
        return -1;
    }

    switch (ret)
    {
        case SDP_CLI_CH_MULTI(SDP_CLI_CH_MULTI_UP) :
            cli_line_switch(sdp_cli, 1);
            break;
        
        case SDP_CLI_CH_MULTI(SDP_CLI_CH_MULTI_DOWN) :
            cli_line_switch(sdp_cli, 0);
            break;
                
        case SDP_CLI_CH_MULTI(SDP_CLI_CH_MULTI_LEFT) :
            cli_cursor_move(sdp_cli, 1);
            break;
            
        case SDP_CLI_CH_MULTI(SDP_CLI_CH_MULTI_RIGHT) :
            cli_cursor_move(sdp_cli, 0);
            break;

        case SDP_CLI_CH_MULTI(SDP_CLI_CH_MULTI_DELETE) :
            cli_delete(sdp_cli);
            break;

        case SDP_CLI_CH_MULTI(SDP_CLI_CH_MULTI_INSERT) :
            cli_insert(ch, sdp_cli);
            break;
            
        case SDP_CLI_CH_SINGLE(SDP_CLI_CH_SINGLE_COMPLECTION) :
            cli_completion(sdp_cli);
            break;
            
        case SDP_CLI_CH_SINGLE(SDP_CLI_CH_SINGLE_EXECUTE) :
            cli_execute(sdp_cli);
            break;

        case SDP_CLI_CH_SINGLE(SDP_CLI_CH_SINGLE_HELP) :
            cli_help(sdp_cli);
            break;

        case SDP_CLI_CH_SINGLE(SDP_CLI_CH_SINGLE_SEPARATE) :
            cli_separate(sdp_cli);
            break;
        
        case SDP_CLI_CH_SINGLE(SDP_CLI_CH_SINGLE_BACKSPACE) :
            cli_backspace(sdp_cli);
            break;

        case SDP_CLI_CH_SINGLE(SDP_CLI_CH_SINGLE_GENERAL) :
            cli_general(ch, sdp_cli);
            break;

       default :
            return -1;
    }

    return 0;
}

SDP_CLI_T *cli_init(SDP_CLI_CONFIG_T *sdp_cli_config)
{
    PTR_CHECK_NULL(sdp_cli_config);

    SDP_CLI_T *sdp_cli = NULL;
    SDP_CLI_CHSET_T cli_ch_set = {0};
    struct termios termios;

    sdp_cli = (SDP_CLI_T *)malloc(sizeof(SDP_CLI_T));
    if (!sdp_cli)
    {
        printf("ERROR : failed to create sdp cli context\n");
        return NULL;
    }
    memset(sdp_cli, 0, sizeof(*sdp_cli));

    /* termios configure */
    tcgetattr(STDIN_FILENO, &termios);
    termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &termios);

    /* cli config */
    sdp_cli->config = *sdp_cli_config;

    /* ch config */
    sdp_cli_ch_init(&cli_ch_set);
    sdp_cli->chset = cli_ch_set;

    return sdp_cli;
}

int cli_machine(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_print_line(sdp_cli->config.rowhead, &sdp_cli->line);

    while (1)
    {
        cli_input_char(sdp_cli);
    }
    
    return 0;
}

int cli_exit(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}
