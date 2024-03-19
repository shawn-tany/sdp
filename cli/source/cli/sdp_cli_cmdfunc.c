#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "sdp_cli_cmdfunc.h"

static CLI_CMD_T *g_cli_cmd = NULL;

static int cmd_handle_test(const void *arg, int arg_len)
{
    PTR_CHECK_N1(arg);

    int i = 0;
    CLI_CMD_PARAM_T *params = NULL;

    if (arg_len != sizeof(CLI_CMD_PARAM_T))
    {
        return -1;
    }
    
    params = (CLI_CMD_PARAM_T *)arg;

    printf("\ncmd test : ");

    for (i = 0; i < params->num; ++i)
    {
        printf("%s ", params->buff[i]);
    }

    printf("\n");

    return 0;
}

static int cmd_cmdfunc_show(const void **args, const int *arg_lens, int arg_num)
{
    PTR_CHECK_N1(args);
    PTR_CHECK_N1(arg_lens);

    int i = 0;
    CLI_CMD_NODE_T *node = NULL;

    printf("    ");

    for (i = 0; i < arg_num; ++i)
    {
        node = (CLI_CMD_NODE_T *)args[i];

        printf("%s ", node->buff);
    }    
    printf("\n");

    return 0;
}

static int cli_cmdfunc_list(const void *arg, int arg_len)
{
    PTR_CHECK_N1(g_cli_cmd);
    PTR_CHECK_N1(arg);

    printf("\n\n");

    sdp_trie_path_list(g_cli_cmd->cmd_trie, cmd_cmdfunc_show);

    return 0;
}

static int cli_cmdfunc_exit(const void *arg, int arg_len)
{
    PTR_CHECK_N1(g_cli_cmd);
    PTR_CHECK_N1(arg);

    g_cli_cmd->status.running = 0;

    return 0;
}


int cli_cmdfunc_register(CLI_CMD_T *cli_cmd)
{
    PTR_CHECK_N1(cli_cmd);

    g_cli_cmd = cli_cmd;

    CLI_CMD_START;

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_ADD("ccc", "ccctest");
    CLI_CMD_ADD("ddd", "dddtest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_ADD("ccc", "ccctest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_ADD("fff", "ffftest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    
    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_ADD("bbbb", "bbbtest");
    CLI_CMD_ADD("fff", "ffftest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aaa", "aaatest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aa", "aatest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("aab", "aatest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("bbb", "bbbtest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("list", "show all command");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cli_cmdfunc_list);

    CLI_CMD_NEW;
    CLI_CMD_ADD("exit", "exit sdp command line");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cli_cmdfunc_exit);
    
    CLI_CMD_END;

    return 0;
}
