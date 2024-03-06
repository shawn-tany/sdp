#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "dir_traval.h"

#define PTR_CHECK(p)            \
    if (!p)                     \
        return DIR_ERR_PARAM;   \

static DIR_STATUS_T dir_node_enqueue(mlt_queue_t *queue, const char *fname, int is_dir, int depth)
{    
    PTR_CHECK(queue);
    PTR_CHECK(fname);

    dir_node_t node;

    snprintf(node.d_name, sizeof(node.d_name), "%s", fname);
    node.is_dir = is_dir;
    node.depth = depth;

    if (0 != mlt_enqueue(queue, &node, sizeof(node)))
    {
        return DIR_ERR_ENQUEUE;
    }
    
    return DIR_SUCCESS;
}

static DIR_STATUS_T dir_node_enstack(mlt_stack_t *stack, const char *fname, int is_dir, int depth)
{
    PTR_CHECK(stack);
    PTR_CHECK(fname);

    dir_node_t node;

    snprintf(node.d_name, sizeof(node.d_name), "%s", fname);
    node.is_dir = is_dir;
    node.depth = depth;

    if (0 != mlt_enstack(stack, &node, sizeof(node)))
    {
        return DIR_ERR_ENSTACK;
    }
    
    return DIR_SUCCESS;
}

DIR_STATUS_T dir_init(dir_ctx_t **ctx, const char *path, int file_num)
{
    PTR_CHECK(ctx);
    PTR_CHECK(path);

    DIR_STATUS_T ret = DIR_SUCCESS;
    struct stat st;

    if (access(path, F_OK))
    {
        ret = DIR_ERR_NOSUCH;
        goto TRAVAL_FAILED;
    }

    (*ctx) = (dir_ctx_t *)malloc(sizeof(dir_ctx_t));

    if (!(*ctx))
    {
        ret = DIR_ERR_MEM;
        goto TRAVAL_FAILED;
    }

    (*ctx)->queue = mlt_queue_create(file_num, sizeof(dir_node_t));

    if (!(*ctx)->queue)
    {
        ret = DIR_ERR_CRTQUEUE;
        goto TRAVAL_FAILED;
    }

    (*ctx)->stack = mlt_stack_create(file_num, sizeof(dir_node_t));

    if (!(*ctx)->stack) 
    {
        ret = DIR_ERR_CRTSTACK;
        goto TRAVAL_FAILED;
    }

    stat(path, &st);

    (*ctx)->is_dir = 1;
    
    if (!S_ISDIR(st.st_mode))
    {
        (*ctx)->is_dir = 0;
        dir_node_enqueue((*ctx)->queue, path, (*ctx)->is_dir, 0);
    }
    
    dir_node_enstack((*ctx)->stack, path, (*ctx)->is_dir, 0);

    (*ctx)->last_depth = -1;

    return DIR_SUCCESS;

TRAVAL_FAILED :

    mlt_stack_free((*ctx)->stack);
    mlt_queue_free((*ctx)->queue);

    free((*ctx));
    (*ctx) = NULL;

    return ret;
}

DIR_STATUS_T dir_uinit(dir_ctx_t *ctx)
{
    if (ctx->queue)
    {
        mlt_queue_free(ctx->queue);
    }

    if (ctx->stack)
    {
        mlt_stack_free(ctx->stack);
    }

    if (ctx)
    {
        free(ctx);
    }

    return DIR_SUCCESS;
}

DIR_STATUS_T dir_push(dir_ctx_t *ctx)
{
    PTR_CHECK(ctx);

    DIR *pDir = NULL;
    char f_name[512] = {0};
    dir_node_t dir_node;
    DIR_STATUS_T ret = DIR_SUCCESS;

    memset(&dir_node, 0, sizeof(dir_node));

    if (!ctx->is_dir)
    {
        return DIR_SUCCESS;
    }

    while (!mlt_stack_top(ctx->stack, &dir_node, sizeof(dir_node)))
    {
        if (ctx->last_depth == dir_node.depth)
        {
            return DIR_DEPTH_OVER;
        }

        mlt_destack(ctx->stack, &dir_node, sizeof(dir_node));
        
        if (DIR_SUCCESS != (ret = dir_node_enqueue(ctx->queue, dir_node.d_name, 1, dir_node.depth)))
        {
            return ret;
        }
    
        pDir = opendir(dir_node.d_name);

        if (!pDir) 
        {
            return DIR_ERR_DIR;
        }

        struct dirent* ent = NULL;

        while ((ent = readdir(pDir)))
        {
            if (!strncmp(ent->d_name, ".", strlen(".")) || 
                !strncmp(ent->d_name, "..", strlen(".."))) 
            {
                continue;
            }

            if ('/' == dir_node.d_name[strlen(dir_node.d_name) - 1])
            {
                snprintf(f_name, sizeof(f_name), "%s%s", dir_node.d_name, ent->d_name);
            } 
            else 
            {
                snprintf(f_name, sizeof(f_name), "%s/%s", dir_node.d_name, ent->d_name);
            }

            if (DT_DIR == ent->d_type) 
            {
                if (DIR_SUCCESS != (ret = dir_node_enstack(ctx->stack, f_name, 1, dir_node.depth + 1)))
                {
                    return ret;
                }
            } 
            else 
            {
                if (DIR_SUCCESS != (ret = dir_node_enqueue(ctx->queue, f_name, 0, dir_node.depth + 1)))
                {
                    return ret;
                }
            }
        }

        closedir(pDir);
    }

    return DIR_SUCCESS;
}

DIR_STATUS_T dir_pop(dir_ctx_t *ctx, dir_node_t *node)
{
    PTR_CHECK(ctx);
    PTR_CHECK(node);
    
    if (ctx->queue->avail == ctx->queue->total) 
    {        
        return DIR_ALL_OVER;
    }

    if (0 != mlt_dequeue(ctx->queue, node, sizeof(*node)))
    {
        return DIR_ERR_DEQUEUE;
    }

    return DIR_SUCCESS;
}
