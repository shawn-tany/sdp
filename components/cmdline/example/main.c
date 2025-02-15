#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sdp_cli.h"
#include "sdp_cli_cmd.h"

static CLI_CMD_T *g_cli_cmd = NULL;

static int sdp_cli_config(CLI_CONFIG_T *cli_config)
{
    PTR_CHECK_N1(cli_config);

    /* cli config */
    cli_config->fdout = STDOUT_FILENO;
    cli_config->rowhead = "<cli>"; /* row head */
    cli_config->prompthead = "\n   option :\n"; /* prompt head */

    return 0;
}

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
    CLI_CMD_ADD("niu", "niutest");
    CLI_CMD_ADD("le", "letest");
    CLI_CMD_ADD("shi", "shitest");
    CLI_CMD_ADD("gang", "gangtest");
    CLI_CMD_ADD("jin", "jintest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("a", "atest");
    CLI_CMD_ADD("biao", "biaotest");
    CLI_CMD_ADD("dang", "dangtest");
    CLI_CMD_ADD("jiao", "jiaotest");
    CLI_CMD_ADD("shou", "shoutest");
    CLI_CMD_REGSTER(cli_cmd->cmd_trie, cmd_handle_test);

    CLI_CMD_NEW;
    CLI_CMD_ADD("wei", "weitest");
    CLI_CMD_ADD("qi", "qitest");
    CLI_CMD_ADD("da", "datest");
    CLI_CMD_ADD("shuai", "shuaitest");
    CLI_CMD_ADD("bi", "bitest");
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

int main()
{
    SDP_CLI_T *sdp_cli = NULL;
    CLI_CONFIG_T cli_config = {0};

    sdp_cli_config(&cli_config);
    
    sdp_cli = sdp_cli_init(&cli_config);

    if (!sdp_cli)
    {
        printf("sdp cli init failed\n");
        return -1;
    }

    cli_cmdfunc_register(sdp_cli->cmds);

    sdp_cli_machine(sdp_cli);

    sdp_cli_exit(sdp_cli);

    return 0;
}
