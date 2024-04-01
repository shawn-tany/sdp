#include <stdio.h>
#include <unistd.h>

#include "boardinfo.h"

#define TERM_HEAD_CLEAR         printf("\033[2J\033[0;0H");
#define TERM_CORS_CLEAR         printf("\033[K");
#define TERM_CORS_MOVE(x, y)    printf("\033[%d;%dH", y, x);

int main()
{
    char buffer[1024] = {0};
    int i = 0;
    int cpu_num = 0;
    int freq = 0;
    int size = 0;
    float usage_rate = 0;
    CPU_STAT_T cpu_stat[8] = {0};
    int times = 100;

    TERM_HEAD_CLEAR;
    
    printf("\n--------------cpu info-----------\n");

    if (0 > bi_cpuinfo_num_get(&cpu_num))
    {
        printf("get cpu num failed\n");
    }
    else
    {
        printf("CPU num                : %d\n", cpu_num);
    }

    for (i = 0; i < cpu_num; ++i)
    {
        if (0 > bi_cpuinfo_name_get(0, buffer, sizeof(buffer)))
        {
            printf("get cpu info failed\n");
        }
        else
        {
            printf("CPU(%d) name            : %s\n", i, buffer);
        }

        if (0 > bi_cpuinfo_freq_get(0, &freq))
        {
            printf("get cpu freq failed\n");
        }
        else
        {
            printf("CPU(%d) freq            : %d MHz\n", i, freq);
        }

        if (0 > bi_cpuinfo_cachesize_get(0, &size))
        {
            printf("get cpu cachesize failed\n");
        }
        else
        {
            printf("CPU(%d) cachesize       : %d K\n", i, size);
        }  
    }
    
    printf("\n--------------mem info-----------\n");
    if (0 > bi_mem_totalsize_get(&size))
    {
        printf("get memory totalsize failed\n");
    }
    else
    {
        printf("Memory totalsize       : %d K\n", size);
    }

    if (0 > bi_mem_freesize_get(&size))
    {
        printf("get memory freesize failed\n");
    }
    else
    {
        printf("Memory freesize        : %d K\n", size);
    }

    if (0 > bi_mem_avaliablesize_get(&size))
    {
        printf("get memory avaliablesize failed\n");
    }
    else
    {
        printf("Memory avaliablesize   : %d K\n", size);
    }

    if (0 > bi_mem_buffersize_get(&size))
    {
        printf("get memory buffersize failed\n");
    }
    else
    {
        printf("Memory buffersize      : %d K\n", size);
    }

    if (0 > bi_mem_cachedsize_get(&size))
    {
        printf("get memory cachedsize failed\n");
    }
    else
    {
        printf("Memory cachedsize      : %d K\n", size);
    }
    
    while (times--)
    {
        TERM_CORS_MOVE(0, 22);
        TERM_CORS_CLEAR;
    
        printf("\n--------------rate info----------\n");
    
        for (i = 0; i < cpu_num; ++i)
        {
            if (0 > bi_cpustat_usagerate_get(0, &cpu_stat[i], &usage_rate))
            {
                printf("get cpu usagerate failed\n");
            }
            else
            {
                printf("CPU(%d) usagerate       : %.2f%%\n", i, usage_rate);
            }
        }

        if (0 > bi_mem_usagerate_get(&usage_rate))
        {
            printf("get memory usagerate failed\n");
        }
        else
        {
            printf("Memory usagerate       : %.2f%%\n", usage_rate);
        }

        sleep(1);
    }

    return 0;
}

