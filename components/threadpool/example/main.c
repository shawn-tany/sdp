#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

#define TEST_NUM 5000

int test[TEST_NUM];

static void *event_test(void *arg, int arg_size)
{
    printf("event(%d) test\n", *(int *)arg);

    usleep(1000 * 100);

    return NULL;
}

int main(int argc, char *argv[])
{
    int i = 0;
    THREAD_POOL_T *pool = NULL;

    pool = thread_pool_create(36, 200);
    if (!pool)
    {
        printf("thread_pool_create failed\n");
        return -1;
    }

    for (i = 0; i < 200; ++i)
    {
        test[i] = i + 1;
    
        if (0 > thread_event_add(pool, event_test, (void *)(&(test[i])), sizeof(test[i])))
        {
            printf("thread_event_add failed\n");
            thread_pool_destory(pool, 1);
            return -1;
        }
    }

    for (i = 200; i < TEST_NUM; ++i)
    {
        test[i] = i + 1;
    
        if (0 > thread_event_add_wait(pool, event_test, (void *)(&(test[i])), sizeof(test[i])))
        {
            printf("thread_event_add timeout\n");
            thread_pool_destory(pool, 1);
            return -1;
        }
    }

    thread_pool_destory(pool, 0);

    return 0;
}
