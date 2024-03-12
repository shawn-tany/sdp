#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "trie.h"

/* insert a new node, and update root node */
static int trie_new_node(TRIE_NODE_T **root, void *data, int size)
{
    int idx = (*root)->node_num;

    /* create new node */
    (*root)->node[idx] = (TRIE_NODE_T *)malloc(sizeof(TRIE_NODE_T) + size);
    if (!(*root)->node[idx])
    {
        return -1;
    }
    memset((*root)->node[idx], 0, sizeof(TRIE_NODE_T) + size);

    /* init new node */
    (*root)->node[idx]->data_len = size;
    memcpy((*root)->node[idx]->data, data, size);
    (*root)->node[idx]->node_num = 0;
    (*root)->node[idx]->depth = (*root)->depth + 1;
    
    /* updata search */
    (*root)->node_num++;
    (*root) = (*root)->node[idx];

    return 0;
}

int trie_strcmp(const void *src, int src_size, const void *dst, int dst_size)
{
    return (src_size == dst_size && !strncmp((char *)src, (char *)dst, src_size));
}

TRIE_ROOT_T *trie_init(trie_cmp_func_t cmp)
{
    TRIE_ROOT_T *root = NULL;

    /* create root node */
    root = (TRIE_ROOT_T *)malloc(sizeof(*root));
    if (!root)
    {
        return NULL;
    }
    memset(root, 0, sizeof(*root));

    root->cmp = cmp;
    root->root.depth = 0;

    return root;
}

int trie_insert(TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num, trie_entry_handle_func_t func)
{
    int pattern_idx = 0;
    int node_idx    = 0;
    int search_flag = 0;

    TRIE_NODE_T *search = &root->root;

    for (pattern_idx = 0; pattern_idx < ele_num; ++pattern_idx)
    {
        /* new node */
        if (!search->node_num)
        {
            if (0 > trie_new_node(&search, pattern[pattern_idx], pattern_len[pattern_idx]))
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
            if (root->cmp(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len))
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
            if (0 > trie_new_node(&search, pattern[pattern_idx], pattern_len[pattern_idx]))
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

TRIE_NODE_T *tire_found(TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num)
{
    int pattern_idx = 0;
    int node_idx    = 0;
    int search_flag = 0;

    TRIE_NODE_T *search = &root->root;

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
            if (root->cmp(pattern[pattern_idx], pattern_len[pattern_idx], 
                            search->node[node_idx]->data, search->node[node_idx]->data_len))
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

int trie_child_entry(TRIE_NODE_T *root, trie_entry_handle_func_t func)
{
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

int trie_path_entry(TRIE_ROOT_T *root, trie_entry_handle_func_t func)
{
    int i   = 0;
    int ret = 0;

    sdp_stack_t *stack = NULL;
    TRIE_NODE_T  *node  = NULL;

    stack = sdp_stack_create(root->total_node_num, sizeof(TRIE_NODE_T *));
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

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(TRIE_NODE_T *));
    }

    while (!sdp_destack(stack, (void *)(&node), sizeof(TRIE_NODE_T *)))
    {
        if (node->node_num)
        {
            for (i = 0; i < node->node_num; ++i)
            {
                sdp_enstack(stack, (void *)(&node->node[i]), sizeof(TRIE_NODE_T *));
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

int trie_path_list(TRIE_ROOT_T *root, trie_path_handle_func_t func)
{
    int i   = 0;
    int ret = 0;

    sdp_stack_t *stack = NULL;
    TRIE_NODE_T  *node  = NULL;

    void *child_data[MAX_CHILD_DEPTH];
    int   child_data_length[MAX_CHILD_DEPTH] = {0};
    int   child_index[MAX_CHILD_DEPTH] = {0};
    int   depth = 0;
    int   idx = 0;

    stack = sdp_stack_create(root->total_node_num, sizeof(TRIE_NODE_T *));
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

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(TRIE_NODE_T *));
    }

    while (!sdp_stack_top(stack, (void *)(&node), sizeof(TRIE_NODE_T *)))
    {
        depth = node->depth;

        child_data[depth - 1] = node->data;
        child_data_length[depth - 1] = node->data_len;

        idx = child_index[depth]++;

        if (node->node_num && idx < node->node_num)
        {
            sdp_enstack(stack, (void *)(&node->node[idx]), sizeof(TRIE_NODE_T *));
            continue;
        }
        else
        {
            sdp_destack(stack, (void *)(&node), sizeof(TRIE_NODE_T *));
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

int trie_each_entry_accord_hierarchy(TRIE_ROOT_T *root, trie_entry_handle_func_t func)
{
    int i   = 0;
    int ret = 0;

    sdp_stack_t *stack = NULL;
    TRIE_NODE_T  *node  = NULL;

    stack = sdp_stack_create(root->total_node_num, sizeof(TRIE_NODE_T *));
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

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(TRIE_NODE_T *));
    }

    while (!sdp_destack(stack, (void *)(&node), sizeof(TRIE_NODE_T *)))
    {
        if (node->node_num)
        {
            for (i = 0; i < node->node_num; ++i)
            {
                sdp_enstack(stack, (void *)(&node->node[i]), sizeof(TRIE_NODE_T *));
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

int trie_uinit(TRIE_ROOT_T *root)
{
    int i = 0;

    sdp_stack_t *stack = NULL;
    TRIE_NODE_T  *node  = NULL;

    stack = sdp_stack_create(root->total_node_num, sizeof(TRIE_NODE_T *));
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

        sdp_enstack(stack, (void *)(&root->root.node[i]), sizeof(TRIE_NODE_T *));
    }

    while (!sdp_destack(stack, (void *)(&node), sizeof(TRIE_NODE_T *)))
    {
        if (node->node_num)
        {
            for (i = 0; i < node->node_num; ++i)
            {
                sdp_enstack(stack, (void *)(&node->node[i]), sizeof(TRIE_NODE_T *));
            }
        }

        /* free node */
        free(node);
    }

    free(root);

    sdp_stack_free(stack);

    return 0;
}