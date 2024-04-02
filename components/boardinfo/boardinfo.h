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
    int total_size;
} DISK_INFO_T;

typedef struct 
{
    int disk_num;
    DISK_INFO_T info[20];
} DISK_STAT_T;

/* cpu info */
int bi_cpu_num_get(int *cpu_num);

int bi_cpu_name_get(int cpu_socket, char *cpu_name, int cpu_name_size);

int bi_cpu_freq_get(int cpu_socket, int *freq);

int bi_cpu_cachesize_get(int cpu_socket, int *size);

int bi_cpu_usagerate_get(int cpu_socket, float *rate);

int bi_cpu_total_usagerate_get(float *rate);

/* memory info */
int bi_mem_usagerate_get(float *rate);

int bi_mem_totalsize_get(int *size);

int bi_mem_avaliablesize_get(int *size);

int bi_mem_freesize_get(int *size);

int bi_mem_buffersize_get(int *size);

int bi_mem_cachedsize_get(int *size);

/* disk info */
int bi_disk_num_get(int *disk_num);

int bi_disk_name_get(int disk_index, char *name, int name_size);

int bi_disk_size_get(int disk_index, int *size);

#endif
