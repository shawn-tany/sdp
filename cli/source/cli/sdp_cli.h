#ifndef SDP_CLI
#define SDP_CLI

#include "common.h"
#include "sdp_cli_ch.h"
#include "sdp_cli_line.h"

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
    SDP_CLI_CONFIG_T config;
    SDP_CLI_LINE_T   line;
    SDP_CLI_CHSET_T  chset;
    
    char param[SDP_CLI_MAX_PARAM_NUM][SDP_CLI_MAX_PARAM_LEN];
} SDP_CLI_T;


int cli_register(SDP_CLI_T *sdp_cli);

SDP_CLI_T *cli_init(SDP_CLI_CONFIG_T *sdp_cli_config);

int cli_machine(SDP_CLI_T *sdp_cli);

int cli_exit(SDP_CLI_T *sdp_cli);

#endif
