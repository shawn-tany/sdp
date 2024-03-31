#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

static void event_test(void *arg, int arg_size)
{
    printf("event(%d) test\n", *(int *)arg);

    sleep(1);
}

int main(int argc, char *argv[])
{
    int i = 0;
    int args[200] = {0};
    THREAD_POOL_T *pool = NULL;

    pool = thread_pool_create(0, 200);
    if (!pool)
    {
        printf("thread_pool_create failed\n");
        return -1;
    }

    for (i = 0; i < 200; ++i)
    {
        args[i] = i + 1;

        if (0 > thread_event_add(pool, event_test, (void *)(&(args[i])), sizeof(args[i])))
        {
            printf("thread_event_add failed\n");
            thread_pool_destory(pool, 1);
            return -1;
        }
    }

    thread_pool_destory(pool, 0);

    return 0;
}