#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BI_CPU_INFO_FILE "/proc/cpuinfo"
#define BI_CPU_STAT_FILE "/proc/stat"

static int bi_cpuinfo_string_get(int cpu_socket, char *key, char *string, int string_size)
{
    FILE *fp;
    char *ptr = NULL;
    char  buffer[1024] = {0};    
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
 
    while (fgets(buffer, sizeof(buffer), fp)) 
    {
        if (strncmp(buffer, "processor", 9)) 
        {
            continue;
        }

        num = strtol(buffer, NULL, 10);
        
        if (num != cpu_socket) 
        {
            continue;
        }

        while (fgets(buffer, sizeof(buffer), fp)) 
        {
            if ('\n' == buffer[0])
            {
                break;
            }
        
            if (strncmp(buffer, key, strlen(key))) 
            {
                continue;
            }

            ptr = strchr(buffer, ':');
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
    char  buffer[1024] = {0};    
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
 
    while (fgets(buffer, sizeof(buffer), fp)) 
    {
        if (strncmp(buffer, "processor", 9)) 
        {
            continue;
        }

        num = strtol(buffer, NULL, 10);
        
        if (num != cpu_socket) 
        {
            continue;
        }

        while (fgets(buffer, sizeof(buffer), fp)) 
        {
            if ('\n' == buffer[0])
            {
                break;
            }
        
            if (strncmp(buffer, key, strlen(key))) 
            {
                continue;
            }

            ptr = strchr(buffer, ':');
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

int bi_cpustat_usagerate_get(int cpu_socket, double *rate)
{
    if (!rate)
    {
        return -1;
    }

    return 0;
}
