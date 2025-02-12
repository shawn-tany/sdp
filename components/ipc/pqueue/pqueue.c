#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "pqueue.h"

#define PQUEUE_MMAP_PATH    "/tmp/pqueue"
#define PQUEUE_MMAP_PLFIX   "PQ_"
#define PQUQUE_PATH_LEN     128

typedef struct 
{
    UINT16_T node_size;
    UINT16_T total;
    UINT16_T refer;
    UINT16_T avali;
    UINT16_T prod;
    UINT16_T cons;
    char     addr[0];
} PQUEUE_HEAD_S;

PQUEUE_S *pqueue_create(char *pq_name, UINT16_T node_size, UINT16_T total)
{
    FD_T           fd         = 0;
    int            err        = 0;
    UINT64_T       totol_size = 0;
    char           pq_path[PQUQUE_PATH_LEN];
    PQUEUE_S      *pqueue     = NULL;
    PQUEUE_HEAD_S *pqhead     = NULL;

    PTR_CHECK_NULL(pq_name);

    DO_ONCE_START;

    pqueue = (PQUEUE_S *)malloc(sizeof(*pqueue));
    if (!pqueue)
    {
        err = 1;
        printf("failed to alloc pqueue\n");
        break;
    }

    if (access(PQUEUE_MMAP_PATH, F_OK))
    {
        mkdir(PQUEUE_MMAP_PATH, 0755);
    }

    snprintf(pqueue->pq_name, sizeof(pqueue->pq_name), "%s", pq_name);
    snprintf(pq_path, sizeof(pq_path), "%s/%s%s", PQUEUE_MMAP_PATH, PQUEUE_MMAP_PLFIX, pqueue->pq_name);

    fd = open(pq_path, O_RDWR | O_CREAT);
    if (0 > fd)
    {
        err = 1;
        printf("failed to open file %s, %s\n", pq_path, strerror(errno));
        break;
    }

    totol_size = sizeof(PQUEUE_HEAD_S) + (node_size * total);

    ftruncate(fd, totol_size);

    pqueue->addr = (PQUEUE_S *)mmap(NULL, totol_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (!pqueue->addr)
    {
        err = 1;
        break;
    }

    pqhead = (PQUEUE_HEAD_S *)pqueue->addr;

    close(fd);

    if (!(pqhead->refer++))
    {
        pqhead->node_size = node_size;
        pqhead->total     = total;
        pqhead->avali     = total;
        pqhead->prod      = 0;
        pqhead->cons      = 0;
    }

    DO_ONCE_END;

    if (err)
    {
        pqueue_destory(pqueue);
        return NULL;
    }

    return pqueue;
}

void pqueue_destory(PQUEUE_S *pqueue)
{
    UINT8_T        clear      = 0;
    UINT64_T       totol_size = 0;
    char           pq_path[PQUQUE_PATH_LEN];
    PQUEUE_HEAD_S *pqhead     = NULL;

    PTR_CHECK_VOID(pqueue);

    pqhead = (PQUEUE_HEAD_S *)pqueue;

    totol_size = sizeof(*pqueue) + (pqhead->node_size * pqhead->total);
    
    if (0 < pqhead->refer || !(--pqhead->refer))
    {
        clear = 1;
    }

    munmap(pqueue->addr, totol_size);
    pqueue->addr = NULL;

    snprintf(pq_path, sizeof(pq_path), "%s/%s%s", PQUEUE_MMAP_PATH, PQUEUE_MMAP_PLFIX, pqueue->pq_name);

    if (clear && !access(pq_path, F_OK))
    {
        remove(pq_path);
    }
}

int pqueue_clear(PQUEUE_S *pqueue)
{
    PQUEUE_HEAD_S *pqhead = NULL;

    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(pqueue->addr);

    pqhead = (PQUEUE_HEAD_S *)pqueue->addr;

    pqhead->prod  = 0;
    pqhead->cons  = 0;
    pqhead->avali = pqhead->total;

    return 0;
}

int pqueue_produce(PQUEUE_S *pqueue, void *data, UINT16_T size)
{
    void          *prod_addr = NULL;
    PQUEUE_HEAD_S *pqhead    = NULL;

    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(data);
    PTR_CHECK_N1(pqueue->addr);

    pqhead = (PQUEUE_HEAD_S *)pqueue->addr;

    if (!pqhead->avali) 
    {
        return -1;
    }

    if (pqhead->node_size < size) 
    {
        return -1;
    }

    prod_addr = pqhead->addr + (pqhead->prod * pqhead->node_size);

    memcpy(prod_addr, data, size);

    pqhead->prod = (++pqhead->prod >= pqhead->total) ? 0 : pqhead->prod;
    pqhead->avali--;

    return 0;
}

static int _pqueue_consume(PQUEUE_S *pqueue, PQUEUE_NODE_S *node)
{
    PQUEUE_HEAD_S *pqhead = NULL;

    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    pqhead = (PQUEUE_HEAD_S *)pqueue->addr;

    if (pqhead->avali >= pqhead->total) 
    {
        return -1;
    }

    node->data = pqhead->addr + (pqhead->cons * pqhead->node_size);
    node->size = pqhead->node_size;

    pqhead->cons = (++pqhead->cons >= pqhead->total) ? 0 : pqhead->cons;
    pqhead->avali++;

    return 0;
}

static int _pqueue_consume_timeout(PQUEUE_S *pqueue, UINT32_T timeout)
{
    PQUEUE_HEAD_S *pqhead = NULL;

    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(pqueue->addr);

    pqhead = (PQUEUE_HEAD_S *)pqueue->addr;

    timeout = timeout * 1000 * 10;

    while ((pqhead->total == pqhead->avali))
    {
        usleep(100);

        if (!(timeout--))
        {
            return -1;
        }
    }

    return 0;
}

static void _pqueue_consume_wait(PQUEUE_S *pqueue)
{
    PQUEUE_HEAD_S *pqhead = NULL;

    PTR_CHECK_VOID(pqueue);
    PTR_CHECK_VOID(pqueue->addr);

    pqhead = (PQUEUE_HEAD_S *)pqueue->addr;

    while ((pqhead->total == pqhead->avali))
    {
        usleep(100);
    }
}

int pqueue_consume(PQUEUE_S *pqueue, PQUEUE_NODE_S *node)
{
    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    return _pqueue_consume(pqueue, node);
}

int pqueue_consume_timeout(PQUEUE_S *pqueue, PQUEUE_NODE_S *node, UINT32_T timeout)
{
    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    if (_pqueue_consume_timeout(pqueue, timeout))
    {
        return -1;
    }

    return pqueue_consume(pqueue, node);
}

int pqueue_consume_wait(PQUEUE_S *pqueue, PQUEUE_NODE_S *node)
{
    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    _pqueue_consume_wait(pqueue);

    return pqueue_consume(pqueue, node);
}
