#ifndef CPU_INFO
#define CPU_INFO

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
    char name[20];
    
    int major_number;
    int minor_number;
    
    int read_success_times;
    int read_merged_times;
    int read_sector_total;
    int read_ms_total;
    
    int write_success_times;
    int write_merged_times;
    int write_sector_total;
    int write_ms_total;

    int io_program_req;
    int io_doing_ms_total;
    int io_doing_ms_weight_total;
} DISK_INFO_T;

typedef struct 
{
    int disk_num;
    DISK_INFO_T info[20];
} DISK_STAT_T;

/* cpu info */
int bi_cpuinfo_num_get(int *cpu_num);

int bi_cpuinfo_name_get(int cpu_socket, char *cpu_name, int cpu_name_size);

int bi_cpuinfo_freq_get(int cpu_socket, int *freq);

int bi_cpuinfo_cachesize_get(int cpu_socket, int *size);

int bi_cpustat_usagerate_get(int cpu_socket, CPU_STAT_T *cpu_stat, float *rate);

/* memory info */
int bi_mem_usagerate_get(float *rate);

int bi_mem_totalsize_get(int *size);

int bi_mem_avaliablesize_get(int *size);

int bi_mem_freesize_get(int *size);

int bi_mem_buffersize_get(int *size);

int bi_mem_cachedsize_get(int *size);

/* disk info */

#endif
