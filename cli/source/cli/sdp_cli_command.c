#include <stdio.h>
#include <string.h>

#include "sdp_cli_command.h"
#include "common.h"

int sdp_cli_cmd_execute(SDP_CLI_CMD_T *sdp_cli_cmd)
{
    PTR_CHECK_N1(sdp_cli_cmd);

    

    return 0;
}

int sdp_cli_cmd_complete(SDP_CLI_CMD_T *sdp_cli_cmd)
{
    PTR_CHECK_N1(sdp_cli_cmd);

    snprintf(sdp_cli_cmd->complete.buff[0], sizeof(sdp_cli_cmd->complete.buff[0]), "aaaa");
    snprintf(sdp_cli_cmd->complete.buff[1], sizeof(sdp_cli_cmd->complete.buff[1]), "bbbb");
    snprintf(sdp_cli_cmd->complete.buff[2], sizeof(sdp_cli_cmd->complete.buff[2]), "cccc");

    sdp_cli_cmd->complete.num = 1;    
    sdp_cli_cmd->complete.separate = 1;

    return 0;
}

