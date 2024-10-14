#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "sdp_cli.h"

static int sdp_cli_separate(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_line_separate(sdp_cli->line);

    return 0;
}

static int sdp_cli_general(char ch, SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_line_insert(ch, sdp_cli->line);
    
    return 0;
}

static int sdp_cli_backspace(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_line_backspace(sdp_cli->line);

    return 0;
}

static int sdp_cli_delete(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_line_delete(sdp_cli->line);

    return 0;
}

static int sdp_cli_insert(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_line_replace(sdp_cli->line);

    return 0;
}

static int sdp_cli_line_switch(SDP_CLI_T *sdp_cli, int direction)
{
    PTR_CHECK_N1(sdp_cli);    

    if (direction)
    {
        cli_line_his_prev(sdp_cli->config.rowhead, sdp_cli->line);
    }
    else
    {
        cli_line_his_next(sdp_cli->config.rowhead, sdp_cli->line);
    }
    
    return 0;
}

static int sdp_cli_cursor_move(SDP_CLI_T *sdp_cli, int direction)
{
    PTR_CHECK_N1(sdp_cli);    

    if (direction)
    {
        cli_line_cursor_left(sdp_cli->line);
    }
    else
    {
        cli_line_cursor_right(sdp_cli->line);
    }

    return 0;
}


/* input option */
static int sdp_cli_help(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    if (-1 == cli_cmd_help(sdp_cli->cmds, sdp_cli->line->cur_line.buff))
    {
        cli_line_enter(sdp_cli->line);
        cli_line_tab(sdp_cli->line);
        cli_line_prints(sdp_cli->line, "ERROR : No such command : ", strlen("ERROR : No such command : "));
        cli_line_prints(sdp_cli->line, sdp_cli->line->cur_line.buff, sdp_cli->line->cur_line.length);

        cli_line_enter(sdp_cli->line);
        cli_line_print_line(sdp_cli->config.rowhead, sdp_cli->line);
    }
    else if (sdp_cli->cmds->prompt.length)
    {
        cli_line_prints(sdp_cli->line, sdp_cli->config.prompthead, strlen(sdp_cli->config.prompthead));
        cli_line_tab(sdp_cli->line);
        cli_line_prints(sdp_cli->line, sdp_cli->cmds->prompt.buff, sdp_cli->cmds->prompt.length);

        cli_line_enter(sdp_cli->line);
        cli_line_print_line(sdp_cli->config.rowhead, sdp_cli->line);
    }

    return 0;
}

static int sdp_cli_execute(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    if (-1 == cli_cmd_execute(sdp_cli->cmds, sdp_cli->line->cur_line.buff))
    {
        cli_line_enter(sdp_cli->line);
        cli_line_tab(sdp_cli->line);
        cli_line_prints(sdp_cli->line, "ERROR : No such command : ", strlen("ERROR : No such command : "));
        cli_line_prints(sdp_cli->line, sdp_cli->line->cur_line.buff, sdp_cli->line->cur_line.length);
    }

    cli_line_new(sdp_cli->config.rowhead, sdp_cli->line);
    
    return 0;
}

static int sdp_cli_completion(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int i = 0;

    cli_cmd_complete(sdp_cli->cmds, sdp_cli->line->cur_line.buff);

    if (sdp_cli->cmds->complete.separate)
    {
        sdp_cli_separate(sdp_cli);
    }

    if (!sdp_cli->cmds->complete.num && !sdp_cli->cmds->complete.enter)
    {
        cli_line_enter(sdp_cli->line);
        cli_line_tab(sdp_cli->line);
        cli_line_prints(sdp_cli->line, "ERROR : No such command : ", strlen("ERROR : No such command : "));
        cli_line_prints(sdp_cli->line, sdp_cli->line->cur_line.buff, sdp_cli->line->cur_line.length);
        cli_line_enter(sdp_cli->line);
        cli_line_print_line(sdp_cli->config.rowhead, sdp_cli->line);
        return -1;
    }
    else if (1 == sdp_cli->cmds->complete.num && !sdp_cli->cmds->complete.enter)
    {
        for (i = 0; i < strlen(sdp_cli->cmds->complete.buff[0]); ++i)
        {
            sdp_cli_general(sdp_cli->cmds->complete.buff[0][i], sdp_cli);
        }
    }
    else
    {    
        cli_line_enter(sdp_cli->line);
    
        for (i = 0; i < sdp_cli->cmds->complete.num; ++i)
        {
            cli_line_tab(sdp_cli->line);
            cli_line_prints(sdp_cli->line, sdp_cli->cmds->complete.buff[i], strlen(sdp_cli->cmds->complete.buff[i]));
        }

        if (sdp_cli->cmds->complete.enter)
        {
            cli_line_tab(sdp_cli->line);
            cli_line_prints(sdp_cli->line, "[Enter]", strlen("[Enter]"));
        }

        cli_line_enter(sdp_cli->line);
        
        cli_line_print_line(sdp_cli->config.rowhead, sdp_cli->line);
    }

    return 0;
}

static int sdp_cli_input_char(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    char ch = 0;
    int ret = 0;

    ret = cli_ch_input(sdp_cli->chset, &ch);
    if (0 > ret)
    {
        return -1;
    }

    switch (ret)
    {
        case CLI_CH_MULTI(CLI_CH_MULTI_UP) :
            sdp_cli_line_switch(sdp_cli, 1);
            break;
        
        case CLI_CH_MULTI(CLI_CH_MULTI_DOWN) :
            sdp_cli_line_switch(sdp_cli, 0);
            break;
                
        case CLI_CH_MULTI(CLI_CH_MULTI_LEFT) :
            sdp_cli_cursor_move(sdp_cli, 1);
            break;
            
        case CLI_CH_MULTI(CLI_CH_MULTI_RIGHT) :
            sdp_cli_cursor_move(sdp_cli, 0);
            break;

        case CLI_CH_MULTI(CLI_CH_MULTI_DELETE) :
            sdp_cli_delete(sdp_cli);
            break;

        case CLI_CH_MULTI(CLI_CH_MULTI_INSERT) :
            sdp_cli_insert(sdp_cli);
            break;
            
        case CLI_CH_SINGLE(CLI_CH_SINGLE_COMPLECTION) :
            sdp_cli_completion(sdp_cli);
            break;
            
        case CLI_CH_SINGLE(CLI_CH_SINGLE_EXECUTE) :
            sdp_cli_execute(sdp_cli);
            break;

        case CLI_CH_SINGLE(CLI_CH_SINGLE_HELP) :
            sdp_cli_help(sdp_cli);
            break;

        case CLI_CH_SINGLE(CLI_CH_SINGLE_SEPARATE) :
            sdp_cli_separate(sdp_cli);
            break;
        
        case CLI_CH_SINGLE(CLI_CH_SINGLE_BACKSPACE) :
        case CLI_CH_SINGLE(CLI_CH_SINGLE_DELETE) :
            sdp_cli_backspace(sdp_cli);
            break;

        case CLI_CH_SINGLE(CLI_CH_SINGLE_GENERAL) :
            sdp_cli_general(ch, sdp_cli);
            break;

       default :
            return -1;
    }

    return 0;
}

SDP_CLI_T *sdp_cli_init(CLI_CONFIG_T *sdp_cli_config)
{
    PTR_CHECK_NULL(sdp_cli_config);

    SDP_CLI_T *sdp_cli = NULL;

    sdp_cli = (SDP_CLI_T *)malloc(sizeof(SDP_CLI_T));
    if (!sdp_cli)
    {
        printf("ERROR : failed to create sdp cli context\n");
        return NULL;
    }
    memset(sdp_cli, 0, sizeof(*sdp_cli));

    /* termios configure */
    tcgetattr(sdp_cli_config->fdout, &sdp_cli_config->termios_org);
    sdp_cli_config->termios_new = sdp_cli_config->termios_org;
    sdp_cli_config->termios_new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(sdp_cli_config->fdout, TCSANOW, &sdp_cli_config->termios_new);

    /* cli config */
    sdp_cli->config = *sdp_cli_config;

    /* ch config */
    sdp_cli->chset = cli_ch_init();
    if (!sdp_cli->chset)
    {
        free(sdp_cli);
        printf("ERROR : failed to create sdp cli chset\n");
        return NULL;
    }

    /* line init */
    sdp_cli->line = cli_line_init(sdp_cli_config->fdout);
    if (!sdp_cli->line)
    {
        free(sdp_cli);
        cli_ch_uninit(sdp_cli->chset);
        printf("ERROR : failed to create sdp cli line\n");
        return NULL;
    }

    /* cmd init */
    sdp_cli->cmds = cli_cmd_init();
    if (!sdp_cli->cmds)
    {
        cli_ch_uninit(sdp_cli->chset);
        cli_line_uninit(sdp_cli->line);
        free(sdp_cli);
        printf("ERROR : failed to create sdp cli cmds\n");
        return NULL;
    }

    return sdp_cli;
}

int sdp_cli_machine(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_line_print_line(sdp_cli->config.rowhead, sdp_cli->line);

    sdp_cli->cmds->status.running = 1;

    while (sdp_cli->cmds->status.running)
    {
        sdp_cli_input_char(sdp_cli);
    }
    
    return 0;
}

int sdp_cli_exit(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    tcsetattr(sdp_cli->config.fdout, TCSANOW, &sdp_cli->config.termios_org);

    cli_ch_uninit(sdp_cli->chset);

    cli_line_uninit(sdp_cli->line);

    cli_cmd_uninit(sdp_cli->cmds);

    free(sdp_cli);

    return 0;
}
