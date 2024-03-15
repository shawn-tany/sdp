#ifndef SDP_CLI_COMMAND
#define SDP_CLI_COMMAND

#define SDP_CLI_CMD_MAX_COMPLETE_NUM 16
#define SDP_CLI_CMD_MAX_COMPLETE_LEN 32

typedef int (*cmd_handle_func)(int, char *[]);

typedef struct 
{
    char buff[SDP_CLI_CMD_MAX_COMPLETE_NUM][SDP_CLI_CMD_MAX_COMPLETE_LEN];
    int num;
    int separate;
} SDP_CLI_CMD_COMPLETE_T;

typedef struct 
{
    SDP_CLI_CMD_COMPLETE_T complete;
} SDP_CLI_CMD_T;

int sdp_cli_cmd_complete(SDP_CLI_CMD_T *sdp_cli_cmd);

int sdp_cli_cmd_execute(SDP_CLI_CMD_T *sdp_cli_cmd);

#endif