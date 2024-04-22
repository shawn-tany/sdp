#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/statvfs.h>

#include "boardinfo.h"

/* cpu */
#define BI_CPU_INFO_FILE "/proc/cpuinfo"
#define BI_CPU_STAT_FILE "/proc/stat"

/* memory */
#define BI_MEM_INFO_FILE "/proc/meminfo"

/* disk */
#define BI_DSK_INFO_FILE "/proc/partitions"
#define BI_DSK_STAT_FILE "/proc/diskstats"
#define BI_DSK_MNTS_FILE "/proc/mounts"

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

typedef struct 
{
    struct 
    {
        int major_number;
        int minor_number;
        double size;
        int type;
        char name[20];
        char mount[20];
    } info[20];

    int total_num;
    int disk_num;
} DISKS_INFO_T;

typedef enum 
{
    TYPE_DISK0 = 0,
    TYPE_DISK1,
    TYPE_PART,
} DISK_TYPE_T;

static CPU_STAT_T   g_cpu_stat[MAX_CPU_NUM] = {0};
static CPU_STAT_T   g_cpu_stat_total= {0};
static DISKS_INFO_T g_dinfo;

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
    if (!fp) 
    {    
        perror("fopen");
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
    if (!fp) 
    {    
        perror("fopen");
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

static int bi_cpuinfo_u64_get(int cpu_index, char *key, SIZE_T *value)
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
    if (!fp) 
    {    
        perror("fopen");
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
    if (!fp)
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

static int __attribute__((constructor)) bi_diskinfo_get(void)
{
    FILE *fp = NULL;
    char line[256] = {0};
    char *ptr = NULL;

    int i = 0;
    int index = 0;
    int headline = 1;

    int rm = 0;
    int ro = 0;
    char size_str[20] = {0};
    char type_str[20] = {0};
    char unit = 0;

    int last_major = 0xffff;
    int min_minor_index = 0;

    fp = popen("lsblk", "r");
    if (!fp)
    {
        perror("lsblk");
        return -1;
    }

    while (fgets(line, sizeof(line), fp))
    {
        if (headline)
        {
            headline = 0;
            continue;
        }

        ptr = line;
        index = g_dinfo.total_num;

        while (' ' == ptr[0])
        {
            ptr += 2;
        }

        if (0xffffffe2 == ptr[0])
        {
            /* partition */
            ptr += 6;
        }

        sscanf(ptr, "%s %d:%d %d %s %d %s %s", g_dinfo.info[index].name, 
                &g_dinfo.info[index].major_number, &g_dinfo.info[index].minor_number,
                &rm, size_str, &ro, type_str, g_dinfo.info[index].mount);

        /* size */
        g_dinfo.info[index].size = strtof(size_str, NULL);

        if (strlen(size_str))
        {
            unit = size_str[strlen(size_str) - 1];
        }
        else
        {
            continue;
        }
        
        if ('G' == unit)
        {
            g_dinfo.info[index].size *= (1024 * 1024 * 1024);
        }
        else if ('M' == unit)
        {
            g_dinfo.info[index].size *= (1024 * 1024);
        }
        else if ('K' == unit)
        {
            g_dinfo.info[index].size *= (1024);
        }
        
        /* type */
        if (!strcmp("disk", type_str))
        {
            g_dinfo.info[index].type = TYPE_DISK0;
        }
        else if (!strcmp("part", type_str) || !strcmp("lvm", type_str))
        {
            g_dinfo.info[index].type = TYPE_PART;
        }
        else
        {
            continue;
        }

        g_dinfo.total_num++;
    }

    pclose(fp);

    last_major = g_dinfo.info[0].major_number;
    min_minor_index = 0;

    if (TYPE_DISK0 == g_dinfo.info[0].type)
    {
        g_dinfo.disk_num++;
    }

    for (i = 1; i < g_dinfo.total_num; ++i)
    {
        if (TYPE_DISK0 != g_dinfo.info[i].type)
        {
            continue;
        }

        if ((last_major == g_dinfo.info[i].major_number) &&
            (g_dinfo.info[i].minor_number > g_dinfo.info[min_minor_index].minor_number))
        {
            g_dinfo.info[min_minor_index].type = TYPE_DISK1;
            g_dinfo.info[i].type = TYPE_DISK0;
            min_minor_index = i;
            continue;
        }
        else if (last_major != g_dinfo.info[i].major_number)
        {
            min_minor_index = g_dinfo.info[i].minor_number;
            g_dinfo.disk_num++;
            continue;
        }
    }

    return 0;
}

/*
 * 功能 : 单位转换(bit、byte、KB、MB、GB)
 * 参数 : 
 *      src : 转换前的值
 *      src_unit ：转换前的单位
 *      dst : 转换后的值<输出参数>
 *      dst_unit : 转换后的单位<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
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

/* 
 * 功能 : 返回单位字符串(bit、byte、KB、MB、GB)
 * 参数 ：
 *      unit : 单位类型(    UNIT_TYPE_BIT、UNIT_TYPE_BYTE、UNIT_TYPE_KB、UNIT_TYPE_MB、UNIT_TYPE_GB)
 * 返回值 :
 *      成功返回单位对应的字符串
 *      失败返回"unkown"
 */
char *bi_unit_str(UNIT_TYPE_T unit)
{
    return (UNIT_TYPE_BIT  == unit) ? "bit"  :
           (UNIT_TYPE_BYTE == unit) ? "byte" :
           (UNIT_TYPE_KB   == unit) ? "KB"   :
           (UNIT_TYPE_MB   == unit) ? "MB"   :
           (UNIT_TYPE_GB   == unit) ? "GB"   : "unkown";
}

/*
 * 功能 : 获取cpu核心总数
 * 参数 :
 *      cpu_num : cpu核心数量<输出参数>
 * 返回 :
 *      成功返回0
 *      失败返回-1
 */
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

/* 
 * 功能 : 获取cpu型号
 * 参数 :
 *      cpu_index : cpu核心下标
 *      cpu_name : cpu型号<输出参数>
 *      cpu_name_size : cpu型号的最大长度
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_name_get(int cpu_index, char *cpu_name, int cpu_name_size)
{
    if (!cpu_name)
    {
        return -1;
    }
    
    return bi_cpuinfo_string_get(cpu_index, "model name", cpu_name, cpu_name_size);
}

/* 
 * 功能 : 获取cpu频率
 * 参数 :
 *      cpu_index : cpu核心下标
 *      freq : cpu频率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_freq_get(int cpu_index, int *freq)
{
    if (!freq)
    {
        return -1;
    }
    
    return bi_cpuinfo_int_get(cpu_index, "cpu MHz", freq);
}

/* 
 * 功能 : 获取cpu频率
 * 参数 :
 *      cpu_index : cpu核心下标
 *      size : cpu缓存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_cachesize_get(int cpu_index, SIZE_T *size)
{
    if (!size)
    {
        return -1;
    }
    
    return bi_cpuinfo_u64_get(cpu_index, "cache size", size);    
}

/* 
 * 功能 : 获取单个cpu核心使用率
 * 参数 :
 *      cpu_index : cpu核心下标
 *      rate : cpu占用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
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
        perror("fopen");
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

/* 
 * 功能 : 获取cpu总使用率
 * 参数 :
 *      rate : cpu占用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_total_usagerate_get(float *rate)
{
    if (!rate)
    {
        return -1;
    }

    return bi_cpu_usagerate_get(-1, rate);
}

/* 
 * 功能 : 获取内存使用率
 * 参数 :
 *      rate : 内存使用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
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

/* 
 * 功能 : 获取内存总量
 * 参数 :
 *      size : 内存总量，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_totalsize_get(SIZE_T *size)
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

/* 
 * 功能 : 获取可用物理内存大小
 * 参数 :
 *      size : 可用物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_avaliablesize_get(SIZE_T *size)
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

/* 
 * 功能 : 获取未被使用的物理内存大小
 * 参数 :
 *      size : 未被使用的物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_freesize_get(SIZE_T *size)
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

/* 
 * 功能 : 获取用于块设备缓冲区的物理内存大小
 * 参数 :
 *      size : 用于块设备缓冲区的物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_buffersize_get(SIZE_T *size)
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

/* 
 * 功能 : 获取用于用户空间普通文件页的物理内存大小
 * 参数 :
 *      size : 用于用户空间普通文件页的物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_cachedsize_get(SIZE_T *size)
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

/* 
 * 功能 : 获取磁盘数量
 * 参数 :
 *      disk_num : 磁盘数量<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_num_get(int *disk_num)
{
    if (!disk_num)
    {
        return -1;
    }

    *disk_num = g_dinfo.disk_num;

    return 0;
}

/* 
 * 功能 : 获取磁盘名称<系统下的命名>
 * 参数 :
 *      disk_index : 磁盘下标，0 ~ <disk_num - 1>
 *      name : 系统下的磁盘名称<输出参数>
 *      name_size : 磁盘名称的最大长度
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_name_get(int disk_index, char *name, int name_size)
{
    if (!name)
    {
        return -1;
    }

    int i = 0;
    int index = 0;

    if (disk_index >= ITEM(g_dinfo.info) || disk_index >= g_dinfo.disk_num)
    {
        return -1;
    }

    for (i = 0; i < g_dinfo.total_num; ++i)
    {
        if (TYPE_DISK0 != g_dinfo.info[i].type)
        {
            continue;
        }

        if (index++ == disk_index)
        {
            snprintf(name, name_size, "%s", g_dinfo.info[i].name);
            return 0;
        }
    }

    return -1;
}

/* 
 * 功能 : 获取磁盘容量
 * 参数 :
 *      disk_index : 磁盘下标，0 ~ <disk_num - 1>
 *      size : 磁盘容量，单位bytes<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_size_get(int disk_index, SIZE_T *size)
{
    if (!size)
    {
        return -1;
    }

    int i = 0;
    int index = 0;

    for (i = 0; i < g_dinfo.total_num; ++i)
    {
        if (TYPE_DISK0 != g_dinfo.info[i].type)
        {
            continue;
        }

        if (index++ == disk_index)
        {
            *size = g_dinfo.info[i].size;
            return 0;
        }
    }
    
    return 0;
}

/* 
 * 功能 : 获取磁盘使用率
 * 参数 :
 *      disk_index : 磁盘下标，0 ~ <disk_num - 1>
 *      rate : 磁盘使用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_usagerate_get(int disk_index, float *rate)
{
    if (!rate)
    {
        return -1;
    }

    unsigned long long used_size = 0;
    unsigned long long total_size = 0;
    struct statvfs stats;

    int i = 0;
    int index = 0;

    for (i = 0; i < g_dinfo.total_num; ++i)
    {
        if (index == disk_index)
        {
            i++;
            break;
        }
        else if (TYPE_DISK0 == g_dinfo.info[i].type)
        {
            index++;
            continue;
        }
    }

    for (; i < g_dinfo.total_num; ++i)
    {
        if (TYPE_DISK0 == g_dinfo.info[i].type)
        {
            break;
        }

        if (!strlen(g_dinfo.info[i].mount))
        {
            continue;
        }

        if (!strcmp("[SWAP]", g_dinfo.info[i].mount))
        {
            used_size += g_dinfo.info[i].size;
            total_size += g_dinfo.info[i].size;
        }
        else
        {
            if (0 > statvfs(g_dinfo.info[i].mount, &stats))
            {
                return -1;
            }

            used_size += ((stats.f_blocks - stats.f_bfree) * stats.f_frsize);
            total_size += (stats.f_blocks * stats.f_frsize);
        }
    }
    
    if (total_size)
    {
        *rate = (float)((double)(used_size * 100) / total_size);
    }
    else
    {
        *rate = 0.0;
        return -1;
    }
    
    return 0;
}


