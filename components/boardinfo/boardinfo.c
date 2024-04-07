#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statvfs.h>

#include "boardinfo.h"

#define BI_CPU_INFO_FILE "/proc/cpuinfo"
#define BI_CPU_STAT_FILE "/proc/stat"

#define BI_MEM_INFO_FILE "/proc/meminfo"

#define BI_DSK_INFO_FILE "/proc/partitions"
#define BI_DSK_STAT_FILE "/proc/diskstats"
#define BI_DSK_MNTS_FILE  "/proc/mounts"

#define ITEM(a) (sizeof(a) / sizeof(a[0]))

#define MAX_CPU_NUM 64

typedef struct 
{
    char name[20];
    long user;
    long nice;
    long system;
    long idle;
} CPU_STAT_T;

typedef struct
{
    int mem_total;
    int mem_free;
    int mem_available;
    int mem_buffers;
    int mem_cached;
} MEM_INFO_T;

typedef struct 
{
    int disk_num;

    struct 
    {
        char name[20];
        int major_number;
        int minor_number;
        int total_size;
    } info [20];
} DISKS_INFO_T;

typedef struct 
{
    int disk_num;

    struct 
    {
        int major_number;
        int minor_number;
        
        char name[20];
        
        unsigned long rd_ios;
        unsigned long rd_merges;
        unsigned long rd_sectors;
        unsigned long rd_ticks;
        
        unsigned long wr_ios;
        unsigned long wr_merges;
        unsigned long wr_sectors;
        unsigned long wr_ticks;
    } stat[20];
} DISKS_STAT_T;

static CPU_STAT_T g_cpu_stat[MAX_CPU_NUM] = {0};
static CPU_STAT_T g_cpu_stat_total= {0};

static struct 
{
    char *prefix;
    int prefix_len;
} g_disk_prefix_table[] = {
    { "sd", 2 },
    { "mmcblk", 6 }
};

static int bi_cpuinfo_string_get(int cpu_index, char *key, char *string, int string_size)
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

        ptr = strchr(line, ':');
        if (!ptr)
        {
            break;
        }

        num = strtol(ptr + 1, NULL, 10);
        
        if (num != cpu_index) 
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

static int bi_cpuinfo_int_get(int cpu_index, char *key, int *value)
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

        ptr = strchr(line, ':');
        if (!ptr)
        {
            break;
        }

        num = strtol(ptr + 1, NULL, 10);
        
        if (num != cpu_index) 
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

static int bi_diskinfo_get(DISKS_INFO_T *dinfo)
{
    if (!dinfo)
    {
        return -1;
    }

    FILE *fp = NULL;
    char line[256] = {0};

    int i = 0;
    int index = 0;
    int headline = 1;

    fp = fopen (BI_DSK_INFO_FILE, "r");
    if (!fp)
    {
        perror("fopen");
        return -1;
    }

    dinfo->disk_num = 0;

    while (fgets(line, sizeof(line), fp))
    {
        if (headline)
        {
            headline = 0;
            continue;
        }
    
        if ('#' == line[0] || '\n' == line[0])
        {
            continue;
        }
        
        index = dinfo->disk_num;
        
        sscanf(line, "%d %d %d %s", 
                &dinfo->info[index].major_number, &dinfo->info[index].minor_number, 
                &dinfo->info[index].total_size, dinfo->info[index].name);

        if (0 != dinfo->info[index].minor_number)
        {
            continue;
        }

        for (i = 0; i < ITEM(g_disk_prefix_table); ++i)
        {
            if (!strncmp(dinfo->info[index].name, g_disk_prefix_table[i].prefix, g_disk_prefix_table[i].prefix_len))
            {
                break;
            }
        }
        
        if (i >= ITEM(g_disk_prefix_table))
        {
            continue;
        }

        dinfo->disk_num++;

        if (dinfo->disk_num >= ITEM(dinfo->info))
        {
            break;
        }
    }
    
    fclose(fp);

    return 0;
}

static int bi_diskstat_get(DISKS_STAT_T *dstat)
{
    if (!dstat)
    {
        return -1;
    }

    FILE *fp;
    char buffer[256];
    int i = 0;
    int index = 0;
    int headline = 1;

    dstat->disk_num = 0;
    
    fp = fopen(BI_DSK_STAT_FILE, "r");
    if (!fp) 
    {
        return -1;
    }
  
    while (fgets(buffer, sizeof(buffer), fp)) 
    {
        if (headline)
        {
            headline = 0;
            continue;
        }
    
        index = dstat->disk_num;
    
        sscanf(buffer, "%d %d %s %lu %lu %lu %lu %lu %lu %lu %lu\n",
                &(dstat->stat[index].major_number), &(dstat->stat[index].major_number), (dstat->stat[index].name),  
                &(dstat->stat[index].rd_ios), &(dstat->stat[index].rd_merges), 
                &(dstat->stat[index].rd_sectors), &(dstat->stat[index].rd_ticks),
                &(dstat->stat[index].wr_ios), &(dstat->stat[index].wr_merges), 
                &(dstat->stat[index].wr_sectors), &(dstat->stat[index].wr_ticks));

        if (0 != dstat->stat[index].minor_number)
        {
            continue;
        }

        for (i = 0; i < ITEM(g_disk_prefix_table); ++i)
        {
            if (!strncmp(dstat->stat[index].name, g_disk_prefix_table[i].prefix, g_disk_prefix_table[i].prefix_len))
            {
                break;
            }
        }
        
        if (i >= ITEM(g_disk_prefix_table))
        {
            continue;
        }
                
        dstat->disk_num++;

        if (dstat->disk_num >= ITEM(dstat->stat))
        {
            break;
        }
    }
 
    fclose(fp);
    
    return 0;
}


int bi_unit_convert(double src, UNIT_TYPE_T src_unit, double *dst, UNIT_TYPE_T *dst_unit)
{
    if (!dst || !dst_unit)
    {
        return -1;
    }

    double value = src;
    int unit = src_unit;

    if (UNIT_TYPE_BIT == src_unit)
    {
        if (!((long long)value % 8) && ((long long)value / 8))
        {
            value /= 8;
            unit++;
        }
        else
        {
            return -1;
        }
    }

    while (1 <= (value / 1024))
    {
        value /= 1024;
        unit++;
    }

    *dst = value;
    *dst_unit = unit;

    return 0;
}

char *bi_unit_str(int unit)
{
    return (UNIT_TYPE_BIT  == unit) ? "bit"  :
           (UNIT_TYPE_BYTE == unit) ? "byte" :
           (UNIT_TYPE_KB   == unit) ? "KB"   :
           (UNIT_TYPE_MB   == unit) ? "MB"   :
           (UNIT_TYPE_GB   == unit) ? "GB"   : "unkown";
}

int bi_cpu_num_get(int *cpu_num)
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

int bi_cpu_name_get(int cpu_index, char *cpu_name, int cpu_name_size)
{
    if (!cpu_name)
    {
        return -1;
    }
    
    return bi_cpuinfo_string_get(cpu_index, "model name", cpu_name, cpu_name_size);
}

int bi_cpu_freq_get(int cpu_index, int *freq)
{
    if (!freq)
    {
        return -1;
    }
    
    return bi_cpuinfo_int_get(cpu_index, "cpu MHz", freq);
}

int bi_cpu_cachesize_get(int cpu_index, int *size)
{
    if (!size)
    {
        return -1;
    }
    
    return bi_cpuinfo_int_get(cpu_index, "cache size", size);    
}

int bi_cpu_usagerate_get(int cpu_index, float *rate)
{
    FILE* fp = NULL;
    char line[256] = {0};
    char buff[32] = {0};
    CPU_STAT_T *last_stat = NULL;
    CPU_STAT_T  cur_stat = {0};
    int ret = -1;

    if (!rate || MAX_CPU_NUM <= cpu_index)
    {
        return -1;
    }

    if (0 > cpu_index)
    {
        last_stat = &g_cpu_stat_total;
        snprintf(buff, sizeof(buff), "cpu");
    }
    else
    {
        last_stat = &g_cpu_stat[cpu_index];
        snprintf(buff, sizeof(buff), "cpu%d", cpu_index);
    }

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

        sscanf(line, "%s %ld %ld %ld %ld", cur_stat.name, &cur_stat.user, 
                &cur_stat.nice, &cur_stat.system, &cur_stat.idle);

        ret = 0;
        break;
    }

    fclose(fp);

    if (0 != ret)
    {
        return ret;
    }

    if (0 > cal_cpu_usagerate(&cur_stat, last_stat, rate))
    {
        return -1;
    }

    *last_stat = cur_stat;

    return ret;
}

int bi_cpu_total_usagerate_get(float *rate)
{
    if (!rate)
    {
        return -1;
    }

    return bi_cpu_usagerate_get(-1, rate);
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

int bi_disk_num_get(int *disk_num)
{
    if (!disk_num)
    {
        return -1;
    }

    DISKS_INFO_T dinfo = {0};

    if (0 > bi_diskinfo_get(&dinfo))
    {
        return -1;
    }

    *disk_num = dinfo.disk_num;

    return 0;
}

int bi_disk_name_get(int disk_index, char *name, int name_size)
{
    if (!name)
    {
        return -1;
    }

    DISKS_INFO_T dinfo = {0};

    if (0 > bi_diskinfo_get(&dinfo))
    {
        return -1;
    }

    if (disk_index >= ITEM(dinfo.info) || disk_index >= dinfo.disk_num)
    {
        return -1;
    }

    snprintf(name, name_size, "%s", dinfo.info[disk_index].name);

    return 0;
}

int bi_disk_size_get(int disk_index, int *size)
{
    if (!size)
    {
        return -1;
    }

    DISKS_INFO_T dinfo = {0};

    if (0 > bi_diskinfo_get(&dinfo))
    {
        return -1;
    }

    if (disk_index >= ITEM(dinfo.info) || disk_index >= dinfo.disk_num)
    {
        return -1;
    }

    *size = dinfo.info[disk_index].total_size;
    
    return 0;
}

int bi_disk_io_usagerate_get(int disk_index, float *rate)
{
    if (!rate)
    {
        return -1;
    }

    DISKS_STAT_T dstat = {0};

    float rd_sec = 0.0;
    float wr_sec = 0.0;

    if (0 > bi_diskstat_get(&dstat))
    {
        return -1;
    }

    if (disk_index >= ITEM(dstat.stat) || disk_index >= dstat.disk_num)
    {
        return -1;
    }

    rd_sec = dstat.stat[disk_index].rd_ios ? (dstat.stat[disk_index].rd_sectors / dstat.stat[disk_index].rd_ios) : 0;

    wr_sec = dstat.stat[disk_index].wr_ios ? (dstat.stat[disk_index].wr_sectors / dstat.stat[disk_index].wr_ios) : 0;

    *rate = (rd_sec + wr_sec) / 2;
    
    return 0;
}

int bi_disk_usagerate_get(int disk_index, float *rate)
{
    if (!rate)
    {
        return -1;
    }

    FILE *fp = NULL;    
    DISKS_INFO_T dinfo = {0};
    char buffer[256] = {0};
    char diskname[256] = {0};
    char mountpath[256] = {0};
    char dstdiskname[256] = {0};
    int ret = -1;
    int i = 0;
    int headline = 1;
    unsigned long long used_size = 0;
    unsigned long long total_size = 0;
    struct statvfs stats;

    if (0 > bi_diskinfo_get(&dinfo))
    {
        return -1;
    }
    
    if (disk_index >= ITEM(dinfo.info) || disk_index >= dinfo.disk_num)
    {
        return -1;
    }

    fp = fopen(BI_DSK_MNTS_FILE, "r");
    if (!fp)
    {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (headline)
        {
            headline = 0;
            continue;
        }
    
        sscanf(buffer, "%s %s", diskname, mountpath);

        snprintf(dstdiskname, sizeof(dstdiskname), "/dev/%s", dinfo.info[disk_index].name);

        if (!strcmp(dstdiskname, diskname))
        {
            ret = 0;
        }
        else
        {
            for (i = 1; i < 8; ++i)
            {
                snprintf(dstdiskname, sizeof(dstdiskname), "/dev/%s%d", dinfo.info[disk_index].name, i);
        
                if (!strcmp(dstdiskname, diskname))
                {
                    ret = 0;
                    break;
                }
            }

            if (8 == i)
            {
                continue;
            }
        }
        
        if (0 > statvfs(mountpath, &stats))
        {
            ret = -1;
            break;
        }

        used_size += ((stats.f_blocks - stats.f_bfree) * stats.f_frsize);
        total_size += (stats.f_blocks * stats.f_frsize);
    }

    if (total_size)
    {
        *rate = (float)((double)(used_size * 100) / total_size);
    }
    else
    {
        *rate = 0.0;
    }
    
    return ret;
}


