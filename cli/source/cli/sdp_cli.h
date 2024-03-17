#ifndef SDP_CLI
#define SDP_CLI

#include <termios.h>

#include "common.h"
#include "sdp_cli_ch.h"
#include "sdp_cli_line.h"
#include "sdp_cli_cmd.h"

#define SDP_CLI_MAX_PARAM_LEN 36
#define SDP_CLI_MAX_PARAM_NUM 16

typedef struct 
{
    int   fdout;
    char *rowhead;
    char *prompthead;
} CLI_CONFIG_T;

typedef struct 
{
    CLI_CONFIG_T  config;
    CLI_LINE_T   *line;
    CLI_CHSET_T  *chset;
    CLI_CMD_T    *cmds;
} SDP_CLI_T;

SDP_CLI_T *sdp_cli_init(CLI_CONFIG_T *sdp_cli_config);

int sdp_cli_machine(SDP_CLI_T *sdp_cli);

int sdp_cli_exit(SDP_CLI_T *sdp_cli);

#endif
