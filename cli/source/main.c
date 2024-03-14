#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sdp_cli.h"

static int sdp_cli_config(SDP_CLI_CONFIG_T *cli_config)
{
    PTR_CHECK_N1(cli_config);

    /* cli config */
    cli_config->fdout = STDOUT_FILENO;
    cli_config->rowhead = "<cli>"; /* row head */
    cli_config->prompthead = "\n   option :\n"; /* prompt head */

    return 0;
}

int main()
{
    SDP_CLI_T *sdp_cli = NULL;
    SDP_CLI_CONFIG_T cli_config = {0};

    sdp_cli_config(&cli_config);
    
    sdp_cli = cli_init(&cli_config);

    if (!sdp_cli)
    {
        printf("sdp cli init failed\n");
        return -1;
    }

    cli_machine(sdp_cli);

    cli_exit(sdp_cli);

    return 0;
}
