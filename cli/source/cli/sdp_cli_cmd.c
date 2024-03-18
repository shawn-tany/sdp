#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "sdp_cli_cmd.h"

static int cli_cmd_cmp_insert(const void *src, int src_size, const void *dst, int dst_size)
{
    PTR_CHECK_N1(src);
    PTR_CHECK_N1(dst);

    CLI_CMD_NODE_T *node_src = NULL;
    CLI_CMD_NODE_T *node_dst = NULL;

    if (sizeof(CLI_CMD_NODE_T) != src_size ||
        sizeof(CLI_CMD_NODE_T) != dst_size)
    {
        return -1;
    }

    node_src = (CLI_CMD_NODE_T *)src;        
    node_dst = (CLI_CMD_NODE_T *)dst;

    if ((node_src->buff_len == node_dst->buff_len) &&
        (!strncmp(node_src->buff, node_dst->buff, node_dst->buff_len)))
    {
        return 0;
    }

    return -1;
}

static int cli_cmd_cmp_found_flex(const void *src, int src_size, const void *dst, int dst_size)
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

static int cli_cmd_cmp_found_fuzz(const void *src, int src_size, const void *dst, int dst_size)
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

    if (!strncmp(cmdparam, cmd_node->buff, cmdparam_len))
    {
        return 0;
    }

    return -1;
}

SDP_TRIE_CMP_T cli_cmd_cmpset= 
{
    .cmp_insert = cli_cmd_cmp_insert,
    .cmp_found_flex = cli_cmd_cmp_found_flex,
    .cmp_found_fuzz = cli_cmd_cmp_found_fuzz
};

static int cli_cmd_parse(CLI_CMD_T *cli_cmd, char *cmdstr)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(cmdstr);

    char *ptr   = NULL;
    char *param = cmdstr;
    int   param_len = 0;
    int   param_num = 0;
    
    cli_cmd->param.num = 0;
        
    while (1)
    {
        ptr = strchr(param, ' ');
        if (!ptr)
        {
            if (!strlen(param))
            {
                break;
            }

            param_len = MIN(strlen(param), (sizeof(cli_cmd->param.buff[param_num]) - 1));
        }
        else
        {
            /* param length */
            param_len = MIN((ptr - param), (sizeof(cli_cmd->param.buff[param_num]) - 1));
        }

        /* param number */
        param_num = cli_cmd->param.num;

        /* param value */
        snprintf(cli_cmd->param.buff[param_num], param_len + 1, "%s", param);    
        cli_cmd->param.buff_ptr[param_num] = (cli_cmd->param.buff[param_num]);
        cli_cmd->param.len[param_num] = param_len;
        cli_cmd->param.num++;

        param += (param_len);
        /* updata param */        
        while (*param == ' ')
        {
            param++;
        }
    } 

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

    cli_cmd->cmd_trie = sdp_trie_init(cli_cmd_cmpset);
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
    
    trie_node = sdp_trie_found(cli_cmd->cmd_trie, 
                               (void **)(cli_cmd->param.buff_ptr), 
                               cli_cmd->param.len, 
                               cli_cmd->param.num);
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

    int i = 0;
    int last_param_index = 0;
    int complete_num = 0;
    int length = 0;
    SDP_TRIE_NODE_T *trie_node = NULL;
    SDP_TRIE_NODE_T *complete_node = NULL;
    CLI_CMD_NODE_T  *cmd_root = NULL;
    CLI_CMD_NODE_T  *cmd_node = NULL;

    cli_cmd_parse(cli_cmd, cmdstr);

    cli_cmd->complete.num      = 0;
    cli_cmd->complete.enter    = 0;
    cli_cmd->complete.separate = 0;

    if (strlen(cmdstr))
    {
        trie_node = sdp_trie_found_fuzz(cli_cmd->cmd_trie, 
                                    (void **)(cli_cmd->param.buff_ptr), 
                                    cli_cmd->param.len, 
                                    cli_cmd->param.num);

        if (!trie_node)
        {
            return -1;
        }
    }
    else
    {
        trie_node = &cli_cmd->cmd_trie->root;
    }

    last_param_index = (cli_cmd->param.num - 1);
    if (0 > last_param_index)
    {
        last_param_index = 0;
    }

    if (trie_node->depth == cli_cmd->param.num && cli_cmd->param.len[last_param_index])
    {
        cmd_root = (CLI_CMD_NODE_T *)trie_node->data;
    
        if (cli_cmd->param.len[last_param_index] == cmd_root->buff_len)
        {
            if (trie_node->tail_flag)
            {
                cli_cmd->complete.enter = 1;
            }

            for (i = 0; i < trie_node->node_num; ++i)
            {
                complete_node = trie_node->node[i];
                cmd_node = (CLI_CMD_NODE_T *)complete_node->data;

                complete_num = cli_cmd->complete.num;

                length = MIN((cmd_node->buff_len + 1), sizeof(cli_cmd->complete.buff[complete_num]));
                
                /* complete value */
                snprintf(cli_cmd->complete.buff[complete_num], length, "%s", cmd_node->buff);    
                cli_cmd->complete.buff_ptr[complete_num] = (cli_cmd->complete.buff[complete_num]);
                cli_cmd->complete.num++;

                cli_cmd->complete.separate = 1;
            }
        }
        else
        {
            complete_num = cli_cmd->complete.num;
        
            length = MIN((cmd_root->buff_len - cli_cmd->param.len[last_param_index] + 1), sizeof(cli_cmd->complete.buff[complete_num]));
        
            /* complete value */
            snprintf(cli_cmd->complete.buff[complete_num], length, "%s", cmd_root->buff + cli_cmd->param.len[last_param_index]);    
            cli_cmd->complete.buff_ptr[complete_num] = (cli_cmd->complete.buff[complete_num]);
            cli_cmd->complete.num++;
        }
    }
    /* root */
    else 
    {
        for (i = 0; i < trie_node->node_num; ++i)
        {
            complete_node = trie_node->node[i];
            cmd_node = (CLI_CMD_NODE_T *)complete_node->data;

            if (cli_cmd->param.num && strncmp(cli_cmd->param.buff[last_param_index], cmd_node->buff, cli_cmd->param.len[last_param_index]))
            {
                continue;
            }

            complete_num = cli_cmd->complete.num;

            length = MIN((cmd_node->buff_len + 1), sizeof(cli_cmd->complete.buff[complete_num]));
            
            /* complete value */
            snprintf(cli_cmd->complete.buff[complete_num], length, "%s", cmd_node->buff);    
            cli_cmd->complete.buff_ptr[complete_num] = (cli_cmd->complete.buff[complete_num]);
            cli_cmd->complete.num++;
        }
    }

    return 0;
}

