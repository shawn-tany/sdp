#include <stdio.h>
#include <unistd.h>

#include "cpu.h"

int main()
{
    char buffer[1024] = {0};
    int i = 0;
    int cpu_num = 0;
    int freq = 0;
    int cache_size = 0;
    float usage_rate = 0;
    CPU_STAT_T cpu_stat[8] = {0};

    if (0 > bi_cpuinfo_num_get(&cpu_num))
    {
        printf("get cpu num failed\n");
    }
    else
    {
        printf("CPU num         : %d\n", cpu_num);
    }

    for (i = 0; i < cpu_num; ++i)
    {
        if (0 > bi_cpuinfo_name_get(0, buffer, sizeof(buffer)))
        {
            printf("get cpu info failed\n");
        }
        else
        {
            printf("CPU(%d) name     : %s\n", i, buffer);
        }

        if (0 > bi_cpuinfo_freq_get(0, &freq))
        {
            printf("get cpu freq failed\n");
        }
        else
        {
            printf("CPU(%d) freq     : %d MHz\n", i, freq);
        }

        if (0 > bi_cpuinfo_cachesize_get(0, &cache_size))
        {
            printf("get cpu cachesize failed\n");
        }
        else
        {
            printf("CPU(%d) cachesize: %d K\n", i, cache_size);
        }  
    }

    while (1)
    {
        for (i = 0; i < cpu_num; ++i)
        {
            if (0 > bi_cpustat_usagerate_get(0, &cpu_stat[i], &usage_rate))
            {
                printf("get cpu usagerate failed\n");
            }
            else
            {
                printf("CPU(%d) usagerate: %.2f\n", i, usage_rate);
            }
        }

        sleep(1);
    }

    return 0;
}

