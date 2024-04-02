#ifndef CPU_INFO
#define CPU_INFO

/* cpu info */
int bi_cpu_num_get(int *cpu_num);

int bi_cpu_name_get(int cpu_index, char *cpu_name, int cpu_name_size);

int bi_cpu_freq_get(int cpu_index, int *freq);

int bi_cpu_cachesize_get(int cpu_index, int *size);

int bi_cpu_usagerate_get(int cpu_index, float *rate);

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
