#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "boardinfo.h"

#define BI_CPU_INFO_FILE "/proc/cpuinfo"
#define BI_CPU_STAT_FILE "/proc/stat"

#define BI_MEM_INFO_FILE "/proc/meminfo"

#define BI_DSK_INFO_FILE "/proc/diskstats"

static int bi_cpuinfo_string_get(int cpu_socket, char *key, char *string, int string_size)
{
    FILE *fp;
    char *ptr = NULL;
    char  line[256] = {0};    
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

static int bi_cpuinfo_int_get(int cpu_socket, char *key, int *value)
{
    FILE *fp;
    char *ptr = NULL;
    char  line[256] = {0};    
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

static int cal_cpu_usagerate(CPU_STAT_T *newstat, CPU_STAT_T *oldstat, float *rate)  
{
    if (!newstat || !oldstat || !rate)
    {
        return -1;
    }

    if (!strlen(oldstat->name))
    {
        *rate = 0;
        return 0;
    }

    unsigned long old_time = 0;
    unsigned long new_time = 0;
    unsigned long user_step = 0;
    unsigned long sys_step = 0;
    
    old_time = (unsigned long) (oldstat->user + oldstat->nice + oldstat->system + oldstat->idle);

    new_time = (unsigned long) (newstat->user + newstat->nice + newstat->system + newstat->idle);
    
    user_step = (unsigned long) (newstat->user - oldstat->user);
    
    sys_step = (unsigned long) (newstat->system - oldstat->system);
    
    if((new_time - old_time) != 0)
    {
         *rate = (float)((user_step + sys_step) * 100) / (new_time - old_time);
    }     
    else
    {   
        return -1;
    }

    return 0; 
}

static int bi_meminfo_get(MEM_INFO_T *minfo)
{
    if (!minfo)
    {
        return -1;
    }

    FILE *fp = NULL;
    char line[256] = {0};

    fp = fopen (BI_MEM_INFO_FILE, "r");
    if (0 > fp)
    {
        perror("fopen");
        return -1;
    }

    fgets(line, sizeof(line), fp);
    minfo->mem_total = strtol((line + 15), NULL, 10);

    fgets(line, sizeof(line), fp);
    minfo->mem_free = strtol((line + 15), NULL, 10);

    fgets(line, sizeof(line), fp);
    minfo->mem_available = strtol((line + 15), NULL, 10);

    fgets(line, sizeof(line), fp);
    minfo->mem_buffers = strtol((line + 15), NULL, 10);

    fgets(line, sizeof(line), fp);
    minfo->mem_cached = strtol((line + 15), NULL, 10);
    
    fclose(fp);

    return 0;
}

static int bi_diskinfo_get(DISK_STAT_T *dstat)
{
    if (!dstat)
    {
        return -1;
    }

    FILE *fp = NULL;
    char line[256] = {0};

    int major = 0;
    int minor = 0;

    fp = fopen (BI_DSK_INFO_FILE, "r");
    if (!fp)
    {
        perror("fopen");
        return -1;
    }

    while (fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%d %d", &major, &minor);

        if (0 != minor)
        {
            continue;
        }

        printf("%s\n", line);
    }
    
    fclose(fp);

    return 0;
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

    DISK_STAT_T dstat = {0};

    bi_diskinfo_get(&dstat);

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

        sscanf(line, "cpu %s %ld %ld %ld %ld", cur_stat.name, &cur_stat.user, 
                &cur_stat.nice, &cur_stat.system, &cur_stat.idle);

        ret = 0;
        break;
    }

    fclose(fp);

    if (0 > cal_cpu_usagerate(&cur_stat, cpu_stat, rate))
    {
        return -1;
    }

    *cpu_stat = cur_stat;

    return ret;
}

int bi_mem_usagerate_get(float *rate)
{
    if (!rate)
    {
        return -1;
    }

    MEM_INFO_T minfo = {0};

    if (0 > bi_meminfo_get(&minfo))
    {
        return -1;
    }
    
    *rate = 100 * (float)(minfo.mem_total - (minfo.mem_free + minfo.mem_buffers + minfo.mem_cached)) / minfo.mem_total;

    return 0;
}

int bi_mem_totalsize_get(int *size)
{
    if (!size)
    {
        return -1;
    }

    MEM_INFO_T minfo = {0};

    if (0 > bi_meminfo_get(&minfo))
    {
        return -1;
    }
    
    *size = minfo.mem_total;

    return 0;
}

int bi_mem_avaliablesize_get(int *size)
{
    if (!size)
    {
        return -1;
    }

    MEM_INFO_T minfo = {0};

    if (0 > bi_meminfo_get(&minfo))
    {
        return -1;
    }
    
    *size = minfo.mem_available;

    return 0;
}

int bi_mem_freesize_get(int *size)
{
    if (!size)
    {
        return -1;
    }

    MEM_INFO_T minfo = {0};

    if (0 > bi_meminfo_get(&minfo))
    {
        return -1;
    }
    
    *size = minfo.mem_free;

    return 0;
}

int bi_mem_buffersize_get(int *size)
{
    if (!size)
    {
        return -1;
    }

    MEM_INFO_T minfo = {0};

    if (0 > bi_meminfo_get(&minfo))
    {
        return -1;
    }
    
    *size = minfo.mem_buffers;

    return 0;
}

int bi_mem_cachedsize_get(int *size)
{
    if (!size)
    {
        return -1;
    }

    MEM_INFO_T minfo = {0};

    if (0 > bi_meminfo_get(&minfo))
    {
        return -1;
    }
    
    *size = minfo.mem_cached;

    return 0;
}


