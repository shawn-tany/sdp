#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "sdp_cli_cmd.h"

static int cli_cmd_cmp(const void *src, int src_size, const void *dst, int dst_size)
{
    PTR_CHECK_N1(src);
    PTR_CHECK_N1(dst);

    char *cmdparam = (char *)src;
    int cmdparam_len = src_size;
    CLI_CMD_NODE_T *cmd_node = NULL;

    if (sizeof(*cmd_node) != dst_size)
    {
        return -1;
    }

    cmd_node = (CLI_CMD_NODE_T *)dst;

    if ((cmdparam_len == cmd_node->buff_len) && 
         !strcmp(cmdparam, cmd_node->buff))
    {
        return 0;
    }

    return -1;
}

static int cli_cmd_parse(CLI_CMD_T *cli_cmd, char *cmdstr)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(cmdstr);

    int param_num = 0;
    
    cli_cmd->param.num = 0;

    param_num = cli_cmd->param.num;

    snprintf(cli_cmd->param.buff[param_num], sizeof(cli_cmd->param.buff[param_num]), "%s", cmdstr);
    cli_cmd->param.len[param_num] = strlen(cmdstr);
    cli_cmd->param.num++;

    return 0;
}

CLI_CMD_T *cli_cmd_init(void)
{
    CLI_CMD_T *cli_cmd = NULL;

    cli_cmd = (CLI_CMD_T *)malloc(sizeof(*cli_cmd));
    if (!cli_cmd)
    {
        return NULL;
    }
    memset(cli_cmd, 0, sizeof(*cli_cmd));

    cli_cmd->cmd_trie = sdp_trie_init(cli_cmd_cmp);
    if (!cli_cmd->cmd_trie)
    {
        free(cli_cmd);
        return NULL;
    }

    return cli_cmd;
}

int cli_cmd_uninit(CLI_CMD_T *cli_cmd)
{
    PTR_CHECK_N1(cli_cmd);

    sdp_trie_uinit(cli_cmd->cmd_trie);

    free(cli_cmd);

    return 0;
}

int cli_cmd_execute(CLI_CMD_T *cli_cmd, char *cmdstr)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(cmdstr);

    SDP_TRIE_NODE_T *trie_node = NULL;

    cli_cmd_parse(cli_cmd, cmdstr);

    trie_node = sdp_trie_found(cli_cmd->cmd_trie, (void **)(cli_cmd->param.buff), cli_cmd->param.len, cli_cmd->param.num);
    if (!trie_node)
    {
        return -1;
    }

    if (0 > trie_node->func(&cli_cmd->param, sizeof(cli_cmd->param)))
    {
        return -1;
    }

    return 0;
}

int cli_cmd_complete(CLI_CMD_T *cli_cmd, char *cmdstr)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(cmdstr);

    cli_cmd_parse(cli_cmd, cmdstr);

    return 0;
}

