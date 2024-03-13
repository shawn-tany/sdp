#ifndef SDP_CLI
#define SDP_CLI

#include "common.h"

#define SDP_CLI_MAX_ROWHEAD_LEN 36
#define SDP_CLI_MAX_LINE_LEN  512
#define SDP_CLI_MAX_PARAM_LEN 36
#define SDP_CLI_MAX_PARAM_NUM 16
#define SDP_CLI_CHA

typedef enum 
{
    SDP_CLI_CH_SINGLE_COMPLECTION = 0,
    SDP_CLI_CH_SINGLE_EXECUTE,
    SDP_CLI_CH_SINGLE_HELP,
    SDP_CLI_CH_SINGLE_SEPARATE,    
    SDP_CLI_CH_SINGLE_BACKSPACE,
    SDP_CLI_CH_SINGLE_ESCAPE,
    SDP_CLI_CH_SINGLE_DELETE,
    SDP_CLI_CH_SINGLE_GENERAL,
} SDP_CLI_CHARACTER_SINGLE_T;

typedef enum 
{
    SDP_CLI_CH_MULTI_UP = 0,
    SDP_CLI_CH_MULTI_DOWN,
    SDP_CLI_CH_MULTI_LEFT,
    SDP_CLI_CH_MULTI_RIGHT,
    SDP_CLI_CH_MULTI_NUM,
} SDP_CLI_CHARACTER_MULTI_T;

typedef struct 
{
    int fd;
    char ch_single[SDP_CLI_CH_SINGLE_GENERAL];
    char ch_single[SDP_CLI_CH_MULTI_NUM];
    char *rowhead;
    char *prompthead;
} SDP_CLI_CONFIG_T;;

typedef struct
{
    char buff[SDP_CLI_MAX_LINE_LEN];
    int length;
    int cursor;
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
