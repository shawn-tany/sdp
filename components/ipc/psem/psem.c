#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "psem.h"

#define PSEM_MMAP_PATH "/tmp/psem"

PSEM_S *psem_init(const char *sem_name)
{
    PSEM_S *psem = NULL;

    psem = (PSEM_S *)malloc(sizeof(*psem));
    if (!psem)
    {
        printf("failed to alloc psem\n");
        return NULL;
    }

    memset(psem, 0, sizeof(*psem));

    if (sem_name)
    {
        snprintf(psem->name, sizeof(psem->name), "/%s", sem_name);

        psem->sem = sem_open(psem->name, O_CREAT, 0666, 1);
        if (!psem->sem)
        {
            printf("failed to open sem, %s\n", strerror(errno));
            free(psem);
            return NULL;
        }

        psem->pflag = 1;
    }
    else
    {
        if (0 > sem_init(psem->sem, 0, 1))
        {
            printf("failed to init sem, %s\n", strerror(errno));
            return NULL;
        }
    }

    return psem;
}

int psem_wait(PSEM_S *psem)
{
    PTR_CHECK_N1(psem);

    return sem_wait(psem->sem);
}

int psem_post(PSEM_S *psem)
{
    PTR_CHECK_N1(psem);

    return sem_post(psem->sem);
}

void psem_uninit(PSEM_S *psem)
{
    PTR_CHECK_VOID(psem);

    if (psem->pflag)
    {
        sem_close(psem->sem);
    }
    else
    {
        sem_destroy(psem->sem);
    }

    free(psem);
}