#ifndef SDP_CLI_COMMAND
#define SDP_CLI_COMMAND

#include <string.h>

#include "sdp_trie.h"

#define CLI_CMD_MAX_PARAM_NUM  16
#define CLI_CMD_MAX_PARAM_LEN  32
#define CLI_CMD_MAX_PROMPT_LEN 128

typedef int (*cmd_handle_func)(int, char *[]);

typedef struct 
{
    char  buff[CLI_CMD_MAX_PARAM_NUM][CLI_CMD_MAX_PARAM_LEN];
    char *buff_ptr[CLI_CMD_MAX_PARAM_NUM];
    int   num;
    int   separate;
} CLI_CMD_COMPLETE_T;

typedef struct 
{
    char  buff[CLI_CMD_MAX_PARAM_NUM][CLI_CMD_MAX_PARAM_LEN];
    char *buff_ptr[CLI_CMD_MAX_PARAM_NUM];
    int   len[CLI_CMD_MAX_PARAM_NUM];
    int   num;
} CLI_CMD_PARAM_T;

typedef struct 
{
    int  buff_len;
    int  prompt_len;
    char buff[CLI_CMD_MAX_PARAM_LEN];
    char prompt[CLI_CMD_MAX_PROMPT_LEN];
} CLI_CMD_NODE_T;

typedef struct 
{
    CLI_CMD_COMPLETE_T complete;
    CLI_CMD_PARAM_T    param;
    SDP_TRIE_ROOT_T   *cmd_trie;
} CLI_CMD_T;

#define CLI_CMD_START                               \
{                                                   \
    CLI_CMD_NODE_T *params[CLI_CMD_MAX_PARAM_NUM];  \
    int *param_lens;                                \
    int param_num = 0;                              \
    int i = 0;                                      \
    for (i = 0; i < ITEM(params); ++i)              \
    {                                               \
        params[i] = (CLI_CMD_NODE_T *)malloc(sizeof(CLI_CMD_NODE_T)); \
    }                                               \
    param_lens = (int *)malloc((sizeof(int) * CLI_CMD_MAX_PARAM_NUM));


#define CLI_CMD_NEW                                 \
    for (i = 0; i < ITEM(params); ++i)              \
    {                                               \
        memset(params[i], 0, sizeof(CLI_CMD_NODE_T)); \
        param_lens[i] = 0;                          \
    }                                               \
    param_num = 0;

#define CLI_CMD_ADD(cmdstr, promptstr)              \
    if (param_num < CLI_CMD_MAX_PARAM_NUM)          \
    {                                               \
        snprintf(params[param_num]->buff, sizeof(params[param_num]->buff), "%s", cmdstr); \
        params[param_num]->buff_len = (int)strlen(cmdstr);\
        snprintf(params[param_num]->prompt, sizeof(params[param_num]->prompt), "%s", promptstr); \
        params[param_num]->prompt_len = (int)strlen(promptstr); \
        param_lens[param_num] = sizeof(CLI_CMD_NODE_T); \
        param_num++;                                \
    }

#define CLI_CMD_REGSTER(cmd_trie, func)             \
    sdp_trie_insert(cmd_trie, (void **)params, param_lens, param_num, func);

#define CLI_CMD_END                                 \
    for (i = 0; i < ITEM(params); ++i)              \
    {                                               \
        free(params[i]);                            \
    }                                               \
    free(param_lens);                               \
}

CLI_CMD_T *cli_cmd_init(void);

int cli_cmd_uninit(CLI_CMD_T *cli_cmd);

int cli_cmd_execute(CLI_CMD_T *cli_cmd, char *cmdstr);

int cli_cmd_complete(CLI_CMD_T *cli_cmd, char *cmdstr);

#endif