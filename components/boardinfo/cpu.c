#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "cpu.h"

#define BI_CPU_INFO_FILE "/proc/cpuinfo"
#define BI_CPU_STAT_FILE "/proc/stat"

static int bi_cpuinfo_string_get(int cpu_socket, char *key, char *string, int string_size)
{
    FILE *fp;
    char *ptr = NULL;
    char  line[1024] = {0};    
    int   num = 0;
    int   length = 0;
    int   ret = -1;

    if (!string || !key)
    {
        return -1;
    }
    
    fp = fopen(BI_CPU_INFO_FILE, "r");
    if (fp == NULL) 
    {
        return -1;
    }
 
    while (fgets(line, sizeof(line), fp)) 
    {
        if (strncmp(line, "processor", 9)) 
        {
            continue;
        }

        num = strtol(line, NULL, 10);
        
        if (num != cpu_socket) 
        {
            continue;
        }

        while (fgets(line, sizeof(line), fp)) 
        {
            if ('\n' == line[0])
            {
                break;
            }
        
            if (strncmp(line, key, strlen(key))) 
            {
                continue;
            }

            ptr = strchr(line, ':');
            if (ptr)
            {
                ptr += 2;
                length = strlen(ptr) - 1;
                length = length > string_size ? string_size : length;
                snprintf(string, length, "%s", ptr);
                ret = 0;
            }

            break;
        }

        break;
    }
 
    fclose(fp);
    
    return ret;
}

int bi_cpuinfo_int_get(int cpu_socket, char *key, int *value)
{
    FILE *fp;
    char *ptr = NULL;
    char  line[1024] = {0};    
    int   num = 0;
    int   ret = -1;

    if (!value || !key)
    {
        return -1;
    }
    
    fp = fopen(BI_CPU_INFO_FILE, "r");
    if (fp == NULL) 
    {
        return -1;
    }
 
    while (fgets(line, sizeof(line), fp)) 
    {
        if (strncmp(line, "processor", 9)) 
        {
            continue;
        }

        num = strtol(line, NULL, 10);
        
        if (num != cpu_socket) 
        {
            continue;
        }

        while (fgets(line, sizeof(line), fp)) 
        {
            if ('\n' == line[0])
            {
                break;
            }
        
            if (strncmp(line, key, strlen(key))) 
            {
                continue;
            }

            ptr = strchr(line, ':');
            if (ptr)
            {
                ptr += 2;
                *value = strtol(ptr, NULL, 0);
                ret = 0;
            }

            break;
        }

        break;
    }
 
    fclose(fp);
    
    return ret;
}

int bi_cpuinfo_num_get(int *cpu_num)
{
    if (!cpu_num)
    {
        return -1;
    }

    (*cpu_num) = sysconf(_SC_NPROCESSORS_CONF);

    if (0 > (*cpu_num))
    {
        return -1;
    }

    return 0;
}

int bi_cpuinfo_name_get(int cpu_socket, char *cpu_name, int cpu_name_size)
{
    if (!cpu_name)
    {
        return -1;
    }
    
    return bi_cpuinfo_string_get(cpu_socket, "model name", cpu_name, cpu_name_size);
}

int bi_cpuinfo_freq_get(int cpu_socket, int *freq)
{
    if (!freq)
    {
        return -1;
    }
    
    return bi_cpuinfo_int_get(cpu_socket, "cpu MHz", freq);
}

int bi_cpuinfo_cachesize_get(int cpu_socket, int *size)
{
    if (!size)
    {
        return -1;
    }
    
    return bi_cpuinfo_int_get(cpu_socket, "cache size", size);    
}

int bi_cpustat_usagerate_get(int cpu_socket, CPU_STAT_T *cpu_stat, float *rate)
{
    FILE* fp = NULL;
    char line[256] = {0};
    char buff[32] = {0};
    CPU_STAT_T cur_stat = {0};
    int ret = -1;
    long totol_time_cur = 0;
    long idle_time_cur = 0;
    long totol_time_last = 0;
    long idle_time_last = 0;

    if (!rate || !cpu_stat)
    {
        return -1;
    }

    snprintf(buff, sizeof(buff), "cpu%d", cpu_socket);

    fp = fopen(BI_CPU_STAT_FILE, "r");
    if (!fp) 
    {
        return 1;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (strncmp(line, buff, strlen(buff)))
        {
            continue;
        }

        sscanf(line, "cpu %ld %ld %ld %ld %ld %ld %ld", 
                    &cur_stat.user, &cur_stat.nice, &cur_stat.system, 
                    &cur_stat.idle, &cur_stat.iowait, &cur_stat.irq, &cur_stat.softirq);

        ret = 0;
        break;
    }

    fclose(fp);

    totol_time_last = cpu_stat->user + cpu_stat->nice + cpu_stat->system + 
                      cpu_stat->idle + cpu_stat->iowait + cpu_stat->irq + cpu_stat->softirq;
    idle_time_last = cpu_stat->idle;

    *cpu_stat = cur_stat;

    if (!totol_time_last)
    {
        return -1;
    }
    
    totol_time_cur = cur_stat.user + cur_stat.nice + cur_stat.system + 
                      cur_stat.idle + cur_stat.iowait + cur_stat.irq + cur_stat.softirq;
    idle_time_cur = cur_stat.idle;

    *rate = 100.0 * (1.0 - (float)(idle_time_cur - idle_time_last) / (float)(totol_time_cur - totol_time_last));

    return ret;
}
