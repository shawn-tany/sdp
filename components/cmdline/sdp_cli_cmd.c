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
    
    memset(&cli_cmd->param, 0, sizeof(cli_cmd->param));

    while (*param == ' ')
    {
        param++;
    }
        
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

    if (' ' == cmdstr[strlen(cmdstr) - 1])
    {
        cli_cmd->param.separate = 1;
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

    if (!cli_cmd->param.num)
    {
        return 0;
    }
    
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

int cli_cmd_help(CLI_CMD_T *cli_cmd, char *cmdstr)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(cmdstr);

    SDP_TRIE_NODE_T *trie_node = NULL;
    CLI_CMD_NODE_T  *cmd_node  = NULL;

    cli_cmd_parse(cli_cmd, cmdstr);

    cli_cmd->prompt.length = 0;

    if (!cli_cmd->param.num)
    {
        return 0;
    }
    
    trie_node = sdp_trie_found_incomplete(cli_cmd->cmd_trie, 
                               (void **)(cli_cmd->param.buff_ptr), 
                               cli_cmd->param.len, 
                               cli_cmd->param.num);
    if (!trie_node)
    {
        return -1;
    }

    cmd_node = (CLI_CMD_NODE_T *)trie_node->data;

    snprintf(cli_cmd->prompt.buff, sizeof(cli_cmd->prompt.buff), "%s", cmd_node->prompt);
    cli_cmd->prompt.length = cmd_node->prompt_len;

    return 0;
}


static int cli_cmd_complete_self(CLI_CMD_T *cli_cmd, SDP_TRIE_NODE_T *trie_node_self)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(trie_node_self);

    int i = 0;
    int last_param_index = 0;    
    int last_param_len = 0;
    int complete_num = 0;
    int length = 0;
    SDP_TRIE_NODE_T *trie_node_child = NULL;
    CLI_CMD_NODE_T  *cmd_node_self = NULL;
    CLI_CMD_NODE_T  *cmd_node_child = NULL;

    last_param_index = (cli_cmd->param.num - 1);
    if (0 > last_param_index)
    {
        last_param_index = 0;
    }

    last_param_len = cli_cmd->param.len[last_param_index];

    cmd_node_self = (CLI_CMD_NODE_T *)trie_node_self->data;

    /* Return the node corresponding to the parameter */
    if (last_param_len == cmd_node_self->buff_len)
    {
        if (trie_node_self->tail_flag)
        {
            cli_cmd->complete.enter = 1;
        }

        for (i = 0; i < trie_node_self->node_num; ++i)
        {
            trie_node_child = trie_node_self->node[i];
            cmd_node_child = (CLI_CMD_NODE_T *)trie_node_child->data;

            complete_num = cli_cmd->complete.num;

            length = MIN((cmd_node_child->buff_len + 1), sizeof(cli_cmd->complete.buff[complete_num]));

            if (!cli_cmd->param.separate)
            {
                cli_cmd->complete.separate = 1;
            }
            
            /* complete value */
            snprintf(cli_cmd->complete.buff[complete_num], length, "%s", cmd_node_child->buff);    
            cli_cmd->complete.buff_ptr[complete_num] = (cli_cmd->complete.buff[complete_num]);
            cli_cmd->complete.num++;
        }
    }
    /* Incomplete parameters, complete the current parameters */
    else
    {
        complete_num = cli_cmd->complete.num;
    
        length = MIN((cmd_node_self->buff_len - last_param_len + 1), sizeof(cli_cmd->complete.buff[complete_num]));
    
        /* complete value */
        snprintf(cli_cmd->complete.buff[complete_num], length, "%s", cmd_node_self->buff + last_param_len);    
        cli_cmd->complete.buff_ptr[complete_num] = (cli_cmd->complete.buff[complete_num]);
        cli_cmd->complete.num++;
    }

    return 0;
}

static int cli_cmd_complete_parent(CLI_CMD_T *cli_cmd, SDP_TRIE_NODE_T *trie_node_parent)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(trie_node_parent);

    int i = 0;
    int last_param_index = 0;    
    int last_param_len = 0;
    int complete_num = 0;
    int length = 0;
    SDP_TRIE_NODE_T *trie_node_brother = NULL;
    CLI_CMD_NODE_T  *cmd_node = NULL;

    last_param_index = (cli_cmd->param.num - 1);
    if (0 > last_param_index)
    {
        last_param_index = 0;
    }

    last_param_len = cli_cmd->param.len[last_param_index];

    for (i = 0; i < trie_node_parent->node_num; ++i)
    {
        trie_node_brother = trie_node_parent->node[i];
        cmd_node = (CLI_CMD_NODE_T *)trie_node_brother->data;

        /* Return the parent node corresponding to the parameter */
        if (cli_cmd->param.num && strncmp(cli_cmd->param.buff[last_param_index], cmd_node->buff, last_param_len))
        {
            continue;
        }

        /* Exact match, and it is a complete command */
        if (trie_node_brother->tail_flag && last_param_len == cmd_node->buff_len)
        {
            cli_cmd->complete.enter = 1;

            if (!trie_node_brother->has_child)
            {
                continue;
            }
        }
        
        complete_num = cli_cmd->complete.num;
        
        length = MIN((cmd_node->buff_len + 1), sizeof(cli_cmd->complete.buff[complete_num]));
        
        /* complete value */
        snprintf(cli_cmd->complete.buff[complete_num], length, "%s", cmd_node->buff);    
        cli_cmd->complete.buff_ptr[complete_num] = (cli_cmd->complete.buff[complete_num]);
        cli_cmd->complete.num++;
    }

    return 0;
}

int cli_cmd_complete(CLI_CMD_T *cli_cmd, char *cmdstr)
{
    PTR_CHECK_N1(cli_cmd);
    PTR_CHECK_N1(cmdstr);

    SDP_TRIE_NODE_T *trie_node = NULL;

    cli_cmd_parse(cli_cmd, cmdstr);

    cli_cmd->complete.num      = 0;
    cli_cmd->complete.enter    = 0;
    cli_cmd->complete.separate = 0;

    /* Existing parameters, perform fuzzy matching to obtain command nodes */
    if (cli_cmd->param.num)
    {
        trie_node = sdp_trie_found_fuzz(cli_cmd->cmd_trie, 
                                    (void **)(cli_cmd->param.buff_ptr), 
                                    cli_cmd->param.len, 
                                    cli_cmd->param.num,
                                    !cli_cmd->param.separate);

        if (!trie_node)
        {
            return -1;
        }
    }
    /* There are no parameters, return the root node directly */
    else
    {
        trie_node = &cli_cmd->cmd_trie->root;
    }

    /* Return the node corresponding to the parameter */
    if (trie_node->depth == cli_cmd->param.num && cli_cmd->param.num)
    {
        cli_cmd_complete_self(cli_cmd, trie_node);
    }
    /* Return the parent node corresponding to the parameter */
    else 
    {
        cli_cmd_complete_parent(cli_cmd, trie_node);
    }

    return 0;
}

