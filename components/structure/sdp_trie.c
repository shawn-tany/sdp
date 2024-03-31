#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdp_stack.h"
#include "sdp_trie.h"

/* insert a new node, and update root node */
static int sdp_trie_new_node(SDP_TRIE_NODE_T **root, void *data, int size)
{
    if (!root || !data)
    {
        return -1;
    }

    int idx = (*root)->node_num;

    /* create new node */
    (*root)->node[idx] = (SDP_TRIE_NODE_T *)malloc(sizeof(SDP_TRIE_NODE_T) + size);
    if (!(*root)->node[idx])
    {
        return -1;
    }
    memset((*root)->node[idx], 0, sizeof(SDP_TRIE_NODE_T) + size);

    /* init new node */
    (*root)->node[idx]->data_len = size;
    memcpy((*root)->node[idx]->data, data, size);
    (*root)->node[idx]->node_num = 0;
    (*root)->node[idx]->depth = (*root)->depth + 1;
    (*root)->node[idx]->has_child = 1;
    
    /* updata search */
    (*root)->node_num++;
    (*root) = (*root)->node[idx];

    return 0;
}

int sdp_trie_strcmp(const void *src, int src_size, const void *dst, int dst_size)
{
    if (!src || !dst)
    {
        return -1;
    }

    return !(src_size == dst_size && !strncmp((char *)src, (char *)dst, src_size));
}

SDP_TRIE_ROOT_T *sdp_trie_init(SDP_TRIE_CMP_T cmpset)
{
    SDP_TRIE_ROOT_T *root = NULL;

    /* create root node */
    root = (SDP_TRIE_ROOT_T *)malloc(sizeof(*root));
    if (!root)
    {
        return NULL;
    }
    memset(root, 0, sizeof(*root));

    root->cmp = cmpset;
    root->root.depth = 0;

    return root;
}

int sdp_trie_insert(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num, sdp_trie_entry_handle_func_t func)
{
    if (!root || !pattern || !pattern_len)
    {
        return -1;
    }

    int pattern_idx = 0;
    int node_idx    = 0;
    int search_flag = 0;

    SDP_TRIE_NODE_T *search = &root->root;

    for (pattern_idx = 0; pattern_idx < ele_num; ++pattern_idx)
    {
        /* new node */
        if (!search->node_num)
        {
            if (0 > sdp_trie_new_node(&search, pattern[pattern_idx], pattern_len[pattern_idx]))
            {
                printf("create new node failed\n");
                return -1;
            }
            root->total_node_num++;
            continue;
        }

        search_flag = 0;

        /* search node */
        for (node_idx = 0; node_idx < search->node_num; ++node_idx)
        {
            /* exist */
            if (!(root->cmp.cmp_insert(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len)))
            {
                /* updata search */
                search = search->node[node_idx];
                search_flag = 1;
                break;                
            }
        }

        /* not found */
        if (!search_flag)
        {
            if (0 > sdp_trie_new_node(&search, pattern[pattern_idx], pattern_len[pattern_idx]))
            {
                printf("create new node failed\n");
                return -1;
            }
            root->total_node_num++;
        }
    }

    /* conffict */
    if (search->tail_flag)
    {
        printf("trie path conffict\n");
        return -1;
    }

    search->tail_flag = 1;
    search->func      = func;

    return 0;
}

SDP_TRIE_NODE_T *sdp_trie_found(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num)
{
    if (!root || !pattern || !pattern_len)
    {
        return NULL;
    }

    int pattern_idx = 0;
    int node_idx    = 0;
    int search_flag = 0;

    SDP_TRIE_NODE_T *search = &root->root;

    for (pattern_idx = 0; pattern_idx < ele_num; ++pattern_idx)
    {
        /* not any node */
        if (!search->node_num)
        {
            return NULL;
        }

        search_flag = 0;

        /* search node */
        for (node_idx = 0; node_idx < search->node_num; ++node_idx)
        {
            /* exist */
            if (!(root->cmp.cmp_found_flex(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len)))
            {
                /* updata search */
                search = search->node[node_idx];
                search_flag = 1;
                break;                
            }
        }

        /* not found */
        if (!search_flag)
        {
            return NULL;
        }
    }

    if (!search->tail_flag)
    {
        return NULL;
    }

    return search;
}


SDP_TRIE_NODE_T *sdp_trie_found_incomplete(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num)
{
    if (!root || !pattern || !pattern_len)
    {
        return NULL;
    }

    int pattern_idx = 0;
    int node_idx    = 0;
    int search_flag = 0;

    SDP_TRIE_NODE_T *search = &root->root;

    for (pattern_idx = 0; pattern_idx < ele_num; ++pattern_idx)
    {
        /* not any node */
        if (!search->node_num)
        {
            return NULL;
        }

        search_flag = 0;

        /* search node */
        for (node_idx = 0; node_idx < search->node_num; ++node_idx)
        {
            /* exist */
            if (!(root->cmp.cmp_found_flex(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len)))
            {
                /* updata search */
                search = search->node[node_idx];
                search_flag = 1;
                break;                
            }
        }

        /* not found */
        if (!search_flag)
        {
            return NULL;
        }
    }

    return search;
}

SDP_TRIE_NODE_T *sdp_trie_found_fuzz(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num, int fuzz_flag)
{
    if (!root || !pattern || !pattern_len)
    {
        return NULL;
    }

    int pattern_idx = 0;
    int node_idx    = 0;
    int search_num  = 0;

    SDP_TRIE_NODE_T *search = &root->root;
    SDP_TRIE_NODE_T *tmp = NULL;

    sdp_trie_cmp_func_t last_cmp;

    if (!fuzz_flag)
    {
        last_cmp = root->cmp.cmp_found_flex;
    }
    else 
    {
        last_cmp = root->cmp.cmp_found_fuzz;
    }

    for (pattern_idx = 0; pattern_idx < ele_num; ++pattern_idx)
    {
        /* not any node */
        if (!search->node_num)
        {
            return NULL;
        }

        search_num = 0;

        /* search node */
        for (node_idx = 0; node_idx < search->node_num; ++node_idx)
        {
            /* last shoud be fuzz */
            if ((pattern_idx >= (ele_num - 1)))
            {
                if (!(last_cmp(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len)))
                {
                    /* updata search */
                    search_num++;
                    tmp = search->node[node_idx];
                }
            }
            else
            {
                if (!(root->cmp.cmp_found_flex(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len)))
                {
                    /* updata search */
                    search = search->node[node_idx];
                }
            }
        }
    }

    if (!search_num)
    {
        return NULL;
    }
    else if (1 == search_num)
    {
        search = tmp;
    }

    return search;
}

int sdp_trie_child_entry(SDP_TRIE_NODE_T *root, sdp_trie_entry_handle_func_t func)
{
    if (!root)
    {
        return -1;
    }

    int child_idx = 0; 

    for (child_idx = 0; child_idx < root->node_num; ++child_idx)
    {
        if (root->node[child_idx])
        {
            if (!func)
            {
                continue;
            }

            if (0 > func(root->node[child_idx]->data, root->node[child_idx]->data_len))
            {
                return -1;
            }
        }
    }

    return 0;
}

int sdp_trie_path_entry(SDP_TRIE_ROOT_T *root, sdp_trie_entry_handle_func_t func)
{
    if (!root)
    {
        return -1;
    }

    int i   = 0;
    int ret = 0;

    SDP_STACK_T *stack = NULL;
    SDP_TRIE_NODE_T  *node  = NULL;

    stack = sdp_stack_create(root->total_node_num, sizeof(SDP_TRIE_NODE_T *));
    if (!stack)
    {
        printf("create stack failed\n");
        return -1;
    }

    for (i = 0; i < root->root.node_num; ++i)
    {
        if (!root->root.node[i])
        {
            break;
        }

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(SDP_TRIE_NODE_T *));
    }

    while (!sdp_destack(stack, (void *)(&node), sizeof(SDP_TRIE_NODE_T *)))
    {
        if (node->node_num)
        {
            for (i = 0; i < node->node_num; ++i)
            {
                sdp_enstack(stack, (void *)(&node->node[i]), sizeof(SDP_TRIE_NODE_T *));
            }
        }

        if (!node->tail_flag)
        {
            continue;
        }

        if (func)
        {
            if (0 > func(node->data, node->data_len))
            {
                ret = -1;
                break;
            }
        }
    }

    sdp_stack_free(stack);

    return ret;
}

int sdp_trie_path_list(SDP_TRIE_ROOT_T *root, sdp_trie_path_handle_func_t func)
{
    if (!root)
    {
        return -1;
    }

    int i   = 0;
    int ret = 0;

    SDP_STACK_T *stack = NULL;
    SDP_TRIE_NODE_T  *node  = NULL;

    void *child_data[MAX_CHILD_DEPTH];
    int   child_data_length[MAX_CHILD_DEPTH] = {0};
    int   child_index[MAX_CHILD_DEPTH] = {0};
    int   depth = 0;
    int   idx = 0;

    stack = sdp_stack_create(root->total_node_num, sizeof(SDP_TRIE_NODE_T *));
    if (!stack)
    {
        printf("create stack failed\n");
        return -1;
    }

    for (i = 0; i < root->root.node_num; ++i)
    {
        if (!root->root.node[i])
        {
            break;
        }

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(SDP_TRIE_NODE_T *));
    }

    while (!sdp_stack_top(stack, (void *)(&node), sizeof(SDP_TRIE_NODE_T *)))
    {
        depth = node->depth;

        child_data[depth - 1] = node->data;
        child_data_length[depth - 1] = node->data_len;

        idx = child_index[depth]++;

        if (node->node_num && idx < node->node_num)
        {
            sdp_enstack(stack, (void *)(&node->node[idx]), sizeof(SDP_TRIE_NODE_T *));
            continue;
        }
        else
        {
            sdp_destack(stack, (void *)(&node), sizeof(SDP_TRIE_NODE_T *));
            child_index[depth] = 0;
        }

        if (!node->tail_flag || !func)
        {
            continue;
        }

        if (0 > func((const void **)child_data, child_data_length, depth))
        {
            ret = -1;
            break;
        }
    }

    sdp_stack_free(stack);

    return ret;
}

int sdp_trie_each_entry_accord_hierarchy(SDP_TRIE_ROOT_T *root, sdp_trie_entry_handle_func_t func)
{
    if (!root)
    {
        return -1;
    }

    int i   = 0;
    int ret = 0;

    SDP_STACK_T *stack = NULL;
    SDP_TRIE_NODE_T  *node  = NULL;

    stack = sdp_stack_create(root->total_node_num, sizeof(SDP_TRIE_NODE_T *));
    if (!stack)
    {
        printf("create stack failed\n");
        return -1;
    }

    for (i = 0; i < root->root.node_num; ++i)
    {
        if (!root->root.node[i])
        {
            break;
        }

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(SDP_TRIE_NODE_T *));
    }

    while (!sdp_destack(stack, (void *)(&node), sizeof(SDP_TRIE_NODE_T *)))
    {
        if (node->node_num)
        {
            for (i = 0; i < node->node_num; ++i)
            {
                sdp_enstack(stack, (void *)(&node->node[i]), sizeof(SDP_TRIE_NODE_T *));
            }
        }

        if (!func)
        {
            continue;
        }

        if (0 > func(node->data, node->data_len))
        {
            ret = -1;
            break;
        }
    }

    sdp_stack_free(stack);

    return ret;
}

int sdp_trie_uinit(SDP_TRIE_ROOT_T *root)
{
    if (!root)
    {
        return -1;
    }

    int i = 0;

    SDP_STACK_T *stack = NULL;
    SDP_TRIE_NODE_T  *node  = NULL;

    stack = sdp_stack_create(root->total_node_num, sizeof(SDP_TRIE_NODE_T *));
    if (!stack)
    {
        printf("create stack failed\n");
        return -1;
    }

    for (i = 0; i < root->root.node_num; ++i)
    {
        if (!root->root.node[i])
        {
            break;
        }

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(SDP_TRIE_NODE_T *));
    }

    while (!sdp_destack(stack, (void *)(&node), sizeof(SDP_TRIE_NODE_T *)))
    {
        if (node->node_num)
        {
            for (i = 0; i < node->node_num; ++i)
            {
                sdp_enstack(stack, (void *)(&node->node[i]), sizeof(SDP_TRIE_NODE_T *));
            }
        }

        /* free node */
        free(node);
    }

    free(root);

    sdp_stack_free(stack);

    return 0;
}
