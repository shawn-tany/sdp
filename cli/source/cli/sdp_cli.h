#ifndef SDP_CLI
#define SDP_CLI

#include "common.h"

#define SDP_CLI_MAX_LINE_LEN  512
#define SDP_CLI_MAX_PARAM_LEN 36
#define SDP_CLI_MAX_PARAM_NUM 16

typedef enum 
{
    SDP_CLI_CHARACTER_TYPE_COMPLECTION = 0,
    SDP_CLI_CHARACTER_TYPE_EXECUTE,
    SDP_CLI_CHARACTER_TYPE_HELP,
    SDP_CLI_CHARACTER_TYPE_SEPARATE,
    SDP_CLI_CHARACTER_TYPE_DELETE,
    SDP_CLI_CHARACTER_TYPE_OP_UP,
    SDP_CLI_CHARACTER_TYPE_OP_DOWN,
    SDP_CLI_CHARACTER_TYPE_OP_LEFT,
    SDP_CLI_CHARACTER_TYPE_OP_RIGHT,
    SDP_CLI_CHARACTER_TYPE_GENERAL,
} SDP_CLI_CHARACTER_TYPE_T;

typedef struct 
{
    int fd;
    char character[SDP_CLI_CHARACTER_TYPE_GENERAL];
} SDP_CLI_CONFIG_T;;

typedef struct
{
    char data[SDP_CLI_MAX_LINE_LEN];
    int length;
} SDP_CLI_LINE_T;

typedef struct 
{
    SDP_CLI_CONFIG_T config;
    SDP_CLI_LINE_T line;
    
    char param[SDP_CLI_MAX_PARAM_NUM][SDP_CLI_MAX_PARAM_LEN];
} SDP_CLI_T;

int cli_register(SDP_CLI_T *sdp_cli);

SDP_CLI_T *cli_init(SDP_CLI_CONFIG_T *sdp_cli_config);

int cli_machine(SDP_CLI_T *sdp_cli);

int cli_exit(SDP_CLI_T *sdp_cli);

#endif
