#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pqueue.h"

int main(int argc, char *argv[ ])
{
    int           i  = 0;
    PQUEUE_S     *pq = NULL;
    PQUEUE_NODE_S node;

    pq = pqueue_create("test", sizeof(int), 100);

    if (!pq)
    {
        printf("failed to create pqueue\n");
        return -1;
    }

    if (1 < argc)
    {
        for (; i < 100; ++i)
        {
            pqueue_produce(pq, &i, sizeof(i));
            printf("produce %d\n", i);
        }

        fgetc(stdin);

        for (; i < 200; ++i)
        {
            pqueue_produce(pq, &i, sizeof(i));
            printf("produce %d\n", i);
        }

        fgetc(stdin);

        for (; i < 300; ++i)
        {
            pqueue_produce(pq, &i, sizeof(i));
            printf("produce %d\n", i);
        }

        fgetc(stdin);
    }
    else 
    {
        for (; i < 100; ++i)
        {
            pqueue_consume_timeout(pq, &node, 5);
            printf("consume %d\n", *(int *)node.data);
        }

        for (; i < 200; ++i)
        {
            pqueue_consume_timeout(pq, &node, 5);
            printf("consume %d\n", *(int *)node.data);
        }

        for (; i < 300; ++i)
        {
            pqueue_consume_timeout(pq, &node, 5);
            printf("consume %d\n", *(int *)node.data);
        }
    }

    pqueue_destory(pq);

    return 0;
}
