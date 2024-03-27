#include <stdio.h>

#include "cpu.h"

int main()
{
    char buffer[1024] = {0};
    int  i = 0;
    int  cpu_num = 0;
    int  freq = 0;
    int  cache_size = 0;

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
            printf("get cpu cache_size failed\n");
        }
        else
        {
            printf("CPU(%d) cachesize: %d K\n", i, cache_size);
        }        
    }

    return 0;
}

