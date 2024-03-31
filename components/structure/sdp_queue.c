#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdp_queue.h"

#define PTR_CHECK(p) {  \
    if (!p) {           \
        return -1;      \
    }                   \
}

SDP_QUEUE_T *sdp_queue_create(int node_num, int node_size)
{
    SDP_QUEUE_T *queue = NULL;

    queue = (SDP_QUEUE_T *)malloc(sizeof(SDP_QUEUE_T));

    if (!queue) {
        return NULL;
    }

    queue->addr = malloc(node_num * node_size);

    if (!queue->addr) {
        return NULL;
    }

    memset(queue->addr, 0, node_num * node_size);

    queue->prod = 0;
    queue->cons = 0;
    queue->total = node_num;
    queue->avail = node_num;
    queue->node_size = node_size;

    return queue;
}

int sdp_enqueue(SDP_QUEUE_T *queue, void *node, int size)
{
    PTR_CHECK(queue);
    PTR_CHECK(node);
    PTR_CHECK(queue->addr);

    void *prod_addr = NULL;

    if (size > queue->node_size) {
        return -2;
    }

    if (!queue->avail) {
        return -3;
    }

    if (queue->prod >= queue->total)
    {
        queue->prod = 0;
    }

    prod_addr = queue->addr + (queue->prod * queue->node_size);

    memcpy(prod_addr, node, size);

    queue->prod++;
    
    queue->avail--;

    return 0;
}

int sdp_dequeue(SDP_QUEUE_T *queue, void *node, int size)
{
    PTR_CHECK(queue);
    PTR_CHECK(node);
    PTR_CHECK(queue->addr);

    void *cons_addr = NULL;

    if (queue->avail == queue->total) {
        return -4;
    }

    if (queue->node_size < size) {
        return -5;
    }

    if (queue->cons >= queue->total)
    {
        queue->cons = 0;
    }

    cons_addr = queue->addr + (queue->cons * queue->node_size);

    memcpy(node, cons_addr, size);

    queue->cons++;
    
    queue->avail++;

    return 0;
}

int sdp_queue_head(SDP_QUEUE_T *queue, void *node, int size)
{
    PTR_CHECK(queue);
    PTR_CHECK(node);
    PTR_CHECK(queue->addr);

    void *cons_addr = NULL;

    if (queue->avail == queue->total) {
        return -4;
    }

    if (queue->node_size < size) {
        return -5;
    }

    cons_addr = queue->addr + (queue->cons * queue->node_size);

    memcpy(node, cons_addr, size);

    return 0;
}

void sdp_queue_free(SDP_QUEUE_T *queue)
{
    if (queue) {
        if (queue->addr) {
            free(queue->addr);
            queue->addr = NULL;
        }

        queue->prod = 0;
        queue->cons = 0;
        queue->total = 0;
        queue->avail = 0;
        queue->node_size = 0;

        free(queue);
    }
}

int sdp_queue_empty(SDP_QUEUE_T *queue)
{
    if (!queue)
    {
        return 0;
    }

    return (queue->avail == queue->total);
}

int sdp_queue_full(SDP_QUEUE_T *queue)
{
    if (!queue)
    {
        return 0;
    }

    return (0 == queue->avail);
}

#if 0
int main()
{
    SDP_QUEUE_T *queue = sdp_queue_create(100, sizeof(int));

    if (!queue) {
        printf("faild to create queue\n");
        return -1;
    }

    int i;
    int node;
    int idex = 0;
    int start = 0;
    
    int loop = start + 50;
    for (i = start; i < loop; ++i, start++) {
        if (sdp_enqueue(queue, &i, sizeof(i))) {
            printf("failed to enqueue\n");
            goto FAIL;
        }
    }
    
    while (!sdp_dequeue(queue, &node, sizeof(node))) {
        printf("dequeue[%d] : %d\n", ++idex, node);
    }

    loop = start + 100;
    for (i = start; i < loop; ++i, start++) {
        if (sdp_enqueue(queue, &i, sizeof(i))) {
            printf("failed to enqueue\n");
            goto FAIL;
        }
    }
    
    while (!sdp_dequeue(queue, &node, sizeof(node))) {
        printf("dequeue[%d] : %d\n", ++idex, node);
    }

    loop = start + 100;
    for (i = start; i < loop; ++i, start++) {
        if (sdp_enqueue(queue, &i, sizeof(i))) {
            printf("failed to enqueue\n");
            goto FAIL;
        }
    }
    
    while (!sdp_dequeue(queue, &node, sizeof(node))) {
        printf("dequeue[%d] : %d\n", ++idex, node);
    }

    sdp_queue_free(queue);

    return 0;

FAIL :

    sdp_queue_free(queue);

    return -1;
}
#endif
