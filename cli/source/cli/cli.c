#include "stdio.h"

#include "cli.h"

static int cli_match(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_execute(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_completion(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_separate(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_help(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_general(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_delete_char(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_printf_line(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_reset_line(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_enter_line(SDP_CLI_T *sdp_cli)
{
    return 0;
}

static int cli_input_char(SDP_CLI_T *sdp_cli)
{
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
        cli_general();

        return 0;
    }

    switch (i)
    {
        case SDP_CLI_CHARACTER_TYPE_COMPLECTION :
            cli_completion();
            break;
            
        case SDP_CLI_CHARACTER_TYPE_EXECUTE :
            cli_execute();
            break;
        
        case SDP_CLI_CHARACTER_TYPE_HELP :
            cli_help();
            break;
        
        case SDP_CLI_CHARACTER_TYPE_SEPARATE :
            cli_separate();
            break;
        
        case SDP_CLI_CHARACTER_TYPE_DELETE :
            cli_delete_char();
            break;

        default :
            return -1;
    }

    return 0;
}

int cli_register(SDP_CLI_T *sdp_cli)
{
    return 0;
}

int cli_init(SDP_CLI_T *sdp_cli)
{
    return 0;
}

int cli_machine(SDP_CLI_T *sdp_cli)
{
    return 0;
}

int cli_exit(SDP_CLI_T *sdp_cli)
{
    return 0;
}