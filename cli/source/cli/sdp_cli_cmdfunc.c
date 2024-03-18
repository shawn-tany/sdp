#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "sdp_cli_cmdfunc.h"

static int cmd_handle_test(const void *arg, int arg_len)
{
    PTR_CHECK_N1(arg);

    printf("cmd test\n");

    return 0;
}

static int cmd_cmdfunc_show(const void **args, const int *arg_lens, int arg_num)
{
    PTR_CHECK_N1(args);
    PTR_CHECK_N1(arg_lens);

    return 0;

    int i = 0;
    CLI_CMD_NODE_T *node = NULL;

    for (i = 0; i < arg_num; ++i)
    {
        node = (CLI_CMD_NODE_T *)args[i];

        printf("%s ", node->buff);
    }

    return 0;
}

static int cli_cmdfunc_list(CLI_CMD_T *cli_cmd)
{
    PTR_CHECK_N1(cli_cmd);

    sdp_trie_path_list(cli_cmd->cmd_trie, cmd_cmdfunc_show);

    return 0;
}

int cli_cmdfunc_register(CLI_CMD_T *cli_cmd)
{
    PTR_CHECK_N1(cli_cmd);

    CLI_CMD_START;

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_ADD("ccc", "ccctest");
    CLI_CMD_ADD("ddd", "dddtest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);
    
    CLI_CMD_END;

    cli_cmdfunc_list(cli_cmd);

    return 0;
}