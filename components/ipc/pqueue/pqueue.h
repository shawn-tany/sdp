#ifndef PQUEUE
#define PQUEUE

#include "common.h"

#define PQUQUE_NAME_LEN  64

typedef struct 
{
    char     pq_name[PQUQUE_NAME_LEN];
    UINT16_T node_size;
    UINT16_T total;
    UINT16_T refer;
    UINT16_T avali;
    UINT16_T prod;
    UINT16_T cons;
    char     addr[0];
} PQUEUE_S;

PQUEUE_S *pqueue_create(char *pq_name, UINT16_T node_size, UINT16_T node_num);

void pqueue_destory(PQUEUE_S *pqueue);

int pqueue_produce(PQUEUE_S *pqueue, void *node, UINT16_T size);

int pqueue_consume(PQUEUE_S *pqueue, void *node, UINT16_T size);

int pqueue_consume_timeout(PQUEUE_S *pqueue, void *node, UINT16_T size, UINT32_T timeout);

int pqueue_consume_valid(PQUEUE_S *pqueue);

#endif