#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sdp_cli.h"

int main()
{
    SDP_CLI_T *sdp_cli = NULL;
    SDP_CLI_CONFIG_T sdp_cli_config = {0};

    /* cli config */
    sdp_cli_config.fd = STDOUT_FILENO;
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_COMPLECTION] = 0x09; /* tab */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_EXECUTE]     = 0x0a; /* enter */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_HELP]        = 0x3f; /* ? */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_SEPARATE]    = 0x20; /* whitespace */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_BACKSPACE]   = 0x08; /* backspace */    
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_DELETE]      = 0x7e; /* delete */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_OP_UP]       = 0x41; /* up */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_OP_DOWN]     = 0x42; /* down */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_OP_LEFT]     = 0x44; /* left */
    sdp_cli_config.character[SDP_CLI_CHARACTER_TYPE_OP_RIGHT]    = 0x43; /* right */

    sdp_cli_config.rowhead = "<cli>"; /* row head */
    sdp_cli_config.prompthead = "\n   option :\n"; /* prompt head */
    
    sdp_cli = cli_init(&sdp_cli_config);

    if (!sdp_cli)
    {
        printf("sdp cli init failed\n");
        return -1;
    }

    cli_machine(sdp_cli);

    cli_exit(sdp_cli);

    return 0;
}
