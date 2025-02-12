#ifndef PQUEUE
#define PQUEUE

#include "common.h"

#define PQUQUE_NAME_LEN  64

typedef struct 
{
    void *addr;
    char  pq_name[PQUQUE_NAME_LEN];
} PQUEUE_S;

typedef struct 
{
    void    *data;
    UINT32_T size;
} PQUEUE_NODE_S;

PQUEUE_S *pqueue_create(char *pq_name, UINT16_T node_size, UINT16_T node_num);

void pqueue_destory(PQUEUE_S *pqueue);

int pqueue_clear(PQUEUE_S *pqueue);

int pqueue_produce(PQUEUE_S *pqueue, void *data, UINT16_T size);

int pqueue_consume(PQUEUE_S *pqueue, PQUEUE_NODE_S *node);

int pqueue_consume_timeout(PQUEUE_S *pqueue, PQUEUE_NODE_S *node, UINT32_T timeout);

int pqueue_consume_wait(PQUEUE_S *pqueue, PQUEUE_NODE_S *node);

#endif