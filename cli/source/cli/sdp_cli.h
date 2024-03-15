#ifndef SDP_CLI
#define SDP_CLI

#include "common.h"
#include "sdp_cli_ch.h"
#include "sdp_cli_line.h"
#include "sdp_cli_command.h"

#define SDP_CLI_MAX_PARAM_LEN 36
#define SDP_CLI_MAX_PARAM_NUM 16

typedef struct 
{
    int   fdout;
    char *rowhead;
    char *prompthead;
} SDP_CLI_CONFIG_T;

typedef struct 
{
    char buff[SDP_CLI_MAX_PARAM_LEN];
    int  param_len;
} SDP_CLI_PARAM_T;

typedef struct 
{
    SDP_CLI_PARAM_T param[SDP_CLI_MAX_PARAM_NUM];
    int param_index;
} SDP_CLI_PARAMS_T;

typedef struct 
{
    SDP_CLI_CONFIG_T  config;
    CLI_LINE_T       *line;
    SDP_CLI_CHSET_T   chset;
    SDP_CLI_PARAMS_T  params;
    SDP_CLI_CMD_T     cmds;
} SDP_CLI_T;

SDP_CLI_T *sdp_cli_init(SDP_CLI_CONFIG_T *sdp_cli_config);

int sdp_cli_machine(SDP_CLI_T *sdp_cli);

int sdp_cli_exit(SDP_CLI_T *sdp_cli);

#endif
