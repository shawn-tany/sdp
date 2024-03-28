#ifndef CPU_INFO
#define CPU_INFO

typedef struct 
{
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
} CPU_STAT_T;

int bi_cpuinfo_num_get(int *cpu_num);

int bi_cpuinfo_name_get(int cpu_socket, char *cpu_name, int cpu_name_size);

int bi_cpuinfo_freq_get(int cpu_socket, int *freq);

int bi_cpuinfo_cachesize_get(int cpu_socket, int *size);

int bi_cpustat_usagerate_get(int cpu_socket, CPU_STAT_T *cpu_stat, float *rate);

#endif
