#ifndef PSEM
#define PSEM

#include <semaphore.h>
#include "common.h"

#define PSEM_NAME_LEN  64

typedef struct 
{
    sem_t   *sem;
    UINT8_T  pflag;
    char     name[PSEM_NAME_LEN];
} PSEM_S;

PSEM_S *psem_init(const char *sem_name);

int psem_wait(PSEM_S *psem);

int psem_post(PSEM_S *psem);

void psem_uninit(PSEM_S *psem);

#endif