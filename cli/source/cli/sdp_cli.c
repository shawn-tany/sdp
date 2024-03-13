#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "sdp_cli.h"

static int cli_printf_line(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_reset_line(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_enter_line(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_match(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_execute(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_enter_line(sdp_cli);

    cli_match(sdp_cli);

    UNUSED(sdp_cli);
    
    return 0;
}

static int cli_completion(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_match(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_separate(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_reset_line(sdp_cli);

    cli_printf_line(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_help(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_general(char character, SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int length = sdp_cli->line.length++;

    sdp_cli->line.data[length] = character;

    write(sdp_cli->config.fd, &character, 1);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_delete_char(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

static int cli_input_char(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    int i = 0;
    char character = 0;

    character = getchar();

    for (i = 0; i < SDP_CLI_CHARACTER_TYPE_GENERAL; ++i)
    {
        if (character == sdp_cli->config.character[i])
        {
            break;
        }
    }

    if (SDP_CLI_CHARACTER_TYPE_GENERAL == i)
    {
        cli_general(character, sdp_cli);

        return 0;
    }

    switch (i)
    {
        case SDP_CLI_CHARACTER_TYPE_COMPLECTION :
            cli_completion(sdp_cli);
            break;
            
        case SDP_CLI_CHARACTER_TYPE_EXECUTE :
            cli_execute(sdp_cli);
            break;
        
        case SDP_CLI_CHARACTER_TYPE_HELP :
            cli_help(sdp_cli);
            break;
        
        case SDP_CLI_CHARACTER_TYPE_SEPARATE :
            cli_separate(sdp_cli);
            break;
        
        case SDP_CLI_CHARACTER_TYPE_DELETE :
            cli_delete_char(sdp_cli);
            break;

        default :
            return -1;
    }

    return 0;
}

int cli_register(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}

SDP_CLI_T *cli_init(SDP_CLI_CONFIG_T *sdp_cli_config)
{
    PTR_CHECK_NULL(sdp_cli_config);

    SDP_CLI_T *sdp_cli = NULL;

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
    
    sdp_cli->config = *sdp_cli_config;

    return sdp_cli;
}

int cli_machine(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    cli_input_char(sdp_cli);

    return 0;
}

int cli_exit(SDP_CLI_T *sdp_cli)
{
    PTR_CHECK_N1(sdp_cli);

    UNUSED(sdp_cli);

    return 0;
}
