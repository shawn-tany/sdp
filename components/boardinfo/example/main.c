#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "boardinfo.h"

#define TERM_HEAD_CLEAR         printf("\033[2J\033[0;0H");
#define TERM_CORS_CLEAR         printf("\033[K");
#define TERM_CORS_MOVE(x, y)    printf("\033[%d;%dH", y, x);

int main(int argc, char *argv[ ])
{
    char buffer[1024] = {0};
    int i = 0;
    int cpu_num = 0;
    int disk_num = 0;
    int freq = 0;
    SIZE_T size = 0;
    float usage_rate = 0;
    int lineoffset = 1;
    double cov_value = 0.0;
    UNIT_TYPE_T cov_unit = 0;
    int times = 0;

    if (2 <= argc)
    {
        times = strtol(argv[1], NULL, 0);
    }

    while ((1 == argc) || ((2 >= argc) && (times--)))
    {
        lineoffset = 1;
    
        TERM_HEAD_CLEAR;
    
        printf("\n--------------cpu info-----------\n");

        if (0 > bi_cpu_num_get(&cpu_num))
        {
            printf("get cpu num failed\n");
        }
        else
        {
            printf("CPU num                : %d\n", cpu_num);
        }

        lineoffset += 3;

        for (i = 0; i < cpu_num; ++i)
        {
            if (0 > bi_cpu_name_get(i, buffer, sizeof(buffer)))
            {
                printf("get cpu info failed\n");
            }
            else
            {
                printf("CPU(%d) name            : %s\n", i, buffer);
            }

            if (0 > bi_cpu_freq_get(i, &freq))
            {
                printf("get cpu freq failed\n");
            }
            else
            {
                printf("CPU(%d) freq            : %d MHz\n", i, freq);
            }

            if (0 > bi_cpu_cachesize_get(i, &size))
            {
                printf("get cpu cachesize failed\n");
            }
            else
            {
                bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
                printf("CPU(%d) cachesize       : %.2f %s\n", i, cov_value, bi_unit_str(cov_unit));
            }  

            lineoffset += 3;
        }
        
        printf("\n--------------mem info-----------\n");
        if (0 > bi_mem_totalsize_get(&size))
        {    
            printf("get memory totalsize failed\n");
        }
        else
        {        
            bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
            printf("Memory totalsize       : %.2f %s\n", cov_value, bi_unit_str(cov_unit));
        }

        if (0 > bi_mem_freesize_get(&size))
        {
            printf("get memory freesize failed\n");
        }
        else
        {        
            bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
            printf("Memory freesize        : %.2f %s\n", cov_value, bi_unit_str(cov_unit));
        }

        if (0 > bi_mem_avaliablesize_get(&size))
        {
            printf("get memory avaliablesize failed\n");
        }
        else
        {    
            bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
            printf("Memory avaliablesize   : %.2f %s\n", cov_value, bi_unit_str(cov_unit));
        }

        if (0 > bi_mem_buffersize_get(&size))
        {
            printf("get memory buffersize failed\n");
        }
        else
        {    
            bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
            printf("Memory buffersize      : %.2f %s\n", cov_value, bi_unit_str(cov_unit));
        }

        if (0 > bi_mem_cachedsize_get(&size))
        {
            printf("get memory cachedsize failed\n");
        }
        else
        {    
            bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
            printf("Memory cachedsize      : %.2f %s\n", cov_value, bi_unit_str(cov_unit));
        }
        
        lineoffset += 7;

        printf("\n--------------disk info----------\n");

        if (0 > bi_disk_num_get(&disk_num))
        {
            printf("get disk num failed\n");
        }
        else
        {
            printf("DISK num               : %d\n", disk_num);
        }
    
        lineoffset += 3;

        for (i = 0; i < disk_num; ++i)
        {
            if (0 > bi_disk_name_get(i, buffer, sizeof(buffer)))
            {
                printf("get disk info failed\n");
            }
            else
            {
                printf("DISK(%d) name           : %s\n", i, buffer);
            }

            if (0 > bi_disk_size_get(i, &size))
            {
                printf("get disk size failed\n");
            }
            else
            {        
                bi_unit_convert((double)size, UNIT_TYPE_KB, &cov_value, &cov_unit);
                printf("DISK(%d) size           : %.2f %s\n", i, cov_value, bi_unit_str(cov_unit));
            }

            lineoffset += 2;
        }
    
        TERM_CORS_MOVE(0, lineoffset);
        TERM_CORS_CLEAR;

        printf("\n--------------rate info----------\n");

        if (0 > bi_cpu_total_usagerate_get(&usage_rate))
        {
            printf("get cpu total usagerate failed\n");
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "%3.2f%c", usage_rate, '%');
            printf("CPU total usagerate    : %s\n", buffer);
        }

        for (i = 0; i < cpu_num; ++i)
        {
            if (0 > bi_cpu_usagerate_get(i, &usage_rate))
            {
                printf("get cpu usagerate failed\n");
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "%3.2f%c", usage_rate, '%');
                printf("CPU(%d) usagerate       : %s\n", i, buffer);
            }
        }

        if (0 > bi_mem_usagerate_get(&usage_rate))
        {
            printf("get memory usagerate failed\n");
        }
        else
        {        
            snprintf(buffer, sizeof(buffer), "%3.2f%c", usage_rate, '%');
            printf("Memory usagerate       : %s\n", buffer);
        }        

        for (i = 0; i < disk_num; ++i)
        {
            if (0 > bi_disk_usagerate_get(i, &usage_rate))
            {
                printf("get disk usagerate failed\n");
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "%3.2f%c", usage_rate, '%');
                printf("DISK(%d) usagerate      : %s\n", i, buffer);
            }
        }

        usleep(500 * 1000);
    }

    return 0;
}

