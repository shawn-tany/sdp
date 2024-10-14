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

#define PQUEUE_MMAP_PLFIX     "/tmp/pqueue"
#define PQUQUE_NAME_TOTAL_LEN 128

PQUEUE_S *pqueue_create(char *pq_name, UINT16_T node_size, UINT16_T total)
{
    PTR_CHECK_NULL(pq_name);

    PQUEUE_S *pqueue     = NULL;
    UINT64_T  totol_size = 0;
    char      pq_name_total[PQUQUE_NAME_TOTAL_LEN];
    FD_T      fd  = 0;
    int       err = 0;

    do
    {
        snprintf(pq_name_total, sizeof(pq_name_total), "%s_PQ%s", PQUEUE_MMAP_PLFIX, pq_name);

        fd = open(pq_name_total, O_RDWR | O_CREAT);
        if (0 > fd)
        {
            err = 1;
            printf("failed to open file %s, %s\n", pq_name_total, strerror(errno));
            break;
        }

        totol_size = sizeof(*pqueue) + (node_size * total);

        ftruncate(fd, totol_size);

        pqueue = (PQUEUE_S *)mmap(NULL, totol_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (!pqueue)
        {
            err = 1;
            break;
        }

        close(fd);

        if (!(pqueue->refer++))
        {
            pqueue->node_size = node_size;
            pqueue->total     = total;
            pqueue->avali     = total;
            snprintf(pqueue->pq_name, sizeof(pqueue->pq_name), "%s", pq_name);
            pqueue->prod      = 0;
            pqueue->cons      = 0;
        }

    } while (0);

    if (err)
    {
        pqueue_destory(pqueue);
        return NULL;
    }

    return pqueue;
}

void pqueue_destory(PQUEUE_S *pqueue)
{
    PTR_CHECK_VOID(pqueue);

    UINT64_T  totol_size = 0;
    UINT8_T   clear      = 0;
    char      pq_name_total[PQUQUE_NAME_TOTAL_LEN];

    snprintf(pq_name_total, sizeof(pq_name_total), "%s_PQ%s", PQUEUE_MMAP_PLFIX, pqueue->pq_name);

    totol_size = sizeof(*pqueue) + (pqueue->node_size * pqueue->total);
    
    if (0 < pqueue->refer && !(--pqueue->refer))
    {
        clear = 1;
    }

    munmap(pqueue, totol_size);

    if (clear && !access(pq_name_total, F_OK))
    {
        remove(pq_name_total);
    }
}

int pqueue_consume(PQUEUE_S *pqueue, void *node, UINT16_T size)
{
    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    void *cons_addr = NULL;

    if (pqueue->avali >= pqueue->total) 
    {
        return -1;
    }

    if (size > pqueue->node_size) 
    {
        return -1;
    }

    if (pqueue->cons >= pqueue->total)
    {
        pqueue->cons = 0;
    }

    cons_addr = pqueue->addr + (pqueue->cons * pqueue->node_size);

    memcpy(node, cons_addr, size);

    pqueue->cons++;
    
    pqueue->avali++;

    return 0;
}

int pqueue_produce(PQUEUE_S *pqueue, void *node, UINT16_T size)
{
    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    void *prod_addr = NULL;

    if (!pqueue->avali) 
    {
        return -1;
    }

    if (pqueue->node_size < size) 
    {
        return -1;
    }

    if (pqueue->prod >= pqueue->total)
    {
        pqueue->prod = 0;
    }

    prod_addr = pqueue->addr + (pqueue->prod * pqueue->node_size);

    memcpy(prod_addr, node, size);

    pqueue->prod++;
    
    pqueue->avali--;

    return 0;
}

int pqueue_consume_timeout(PQUEUE_S *pqueue, void *node, UINT16_T size, UINT32_T timeout)
{
    PTR_CHECK_N1(pqueue);
    PTR_CHECK_N1(node);
    PTR_CHECK_N1(pqueue->addr);

    UINT32_T step = 0;

    while ((pqueue->total == pqueue->avali))
    {
        usleep(100);

        if (((step++) == 10) && (timeout--))
        {
            step = 0;
        }

        if (!timeout)
        {
            return -1;
        }
    }

    return pqueue_consume(pqueue, node, size);
}

int pqueue_consume_valid(PQUEUE_S *pqueue)
{
    PTR_CHECK_0(pqueue);

    return !(pqueue->avali == pqueue->total);
}