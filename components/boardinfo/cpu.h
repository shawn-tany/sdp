#ifndef CPU_INFO
#define CPU_INFO

int bi_cpuinfo_num_get(int *cpu_num);

int bi_cpuinfo_name_get(int cpu_socket, char *cpu_name, int cpu_name_size);

int bi_cpuinfo_freq_get(int cpu_socket, int *freq);

int bi_cpuinfo_cachesize_get(int cpu_socket, int *size);

#endif
