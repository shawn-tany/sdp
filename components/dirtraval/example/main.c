#include <stdio.h>
#include <unistd.h>

#include "dir_traval.h"

int main(int argc, char *argv[ ])
{
    char *path = NULL;
    
    if (2 > argc) {
        path = ".";
    }
    else
    {
        path = argv[1];
    }

    if (access(path, F_OK))
    {
        printf("ERROR \"%s\": no such file or directory!\n", path);
        return -1;
    }

    dir_ctx_t *ctx = NULL;
    DIR_STATUS_T ret = DIR_SUCCESS;

    if (DIR_SUCCESS != (ret = dir_init(&ctx, path, 10000)))
    {
        return ret;
    }

    dir_node_t node;
    int i;

    do 
    {
        ret = dir_push(ctx);

        if (DIR_DEPTH_OVER != ret && DIR_SUCCESS != ret)
        {
            goto DIRTRAVEL_FAIL;
        }

        while (DIR_SUCCESS == dir_pop(ctx, &node))
        {
            for (i = 0; i < node.depth; ++i)
            {
                printf("  ");
            }

            if (!node.is_dir)
            {
                printf("|");
            } 
            else 
            {
                printf("+");
            }

            printf("%s\n", node.d_name);
        }
    } while (DIR_DEPTH_OVER == ret);

    dir_uinit(ctx);

    return 0;

DIRTRAVEL_FAIL :

    printf("dir traval failed (0x%x)\n", ret);
    dir_uinit(ctx);

    return 0;
}

