#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "psem.h"

int main(int argc, char *argv[ ])
{
    int     i    = 0;
    PSEM_S *psem = NULL;

    psem = psem_init("test");
    if (!psem)
    {
        printf("failed to init psem\n");
        return -1;
    }

    if (1 < argc)
    {
        DO_LOOP_START;

        psem_wait(psem);

        fgetc(stdin);

        psem_post(psem);

        printf("sem step %d\n", ++i);

        fgetc(stdin);

        DO_LOOP_END;
    }
    else 
    {
        DO_LOOP_START;

        psem_wait(psem);

        printf("sem loop %d\n", ++i);

        psem_post(psem);

        DO_LOOP_END;
    }

    psem_uninit(psem);

    return 0;
}
