#include "cli.h"

int cli_register()
{
    return 0;
}

int cli_match()
{
    return 0;
}

int cli_completion()
{
    return 0;
}

int cli_prompt()
{
    return 0;
}

int cli_input_char(char character, SDP_CLI_T *sdp_cli)
{
    swicth (character)
    {
        case sdp_cli->key.completion :
            break;

        case sdp_cli->key.prompt :
            break;

        case sdp_cli->key.execute :
            break;

        default :
            break;
    }

    return 0;
}

int cli_delete_char()
{
    return 0;
}

int cli_machine()
{
    return 0;
}