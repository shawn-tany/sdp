#ifndef CPU_INFO
#define CPU_INFO

typedef enum 
{
    UNIT_TYPE_BIT = 0,
    UNIT_TYPE_BYTE,
    UNIT_TYPE_KB,
    UNIT_TYPE_MB,
    UNIT_TYPE_GB,
} UNIT_TYPE_T;

typedef unsigned long long SIZE_T;

/* 
 * 功能 : 返回单位字符串(bit、byte、KB、MB、GB)
 * 参数 ：
 *      unit : 单位类型(    UNIT_TYPE_BIT、UNIT_TYPE_BYTE、UNIT_TYPE_KB、UNIT_TYPE_MB、UNIT_TYPE_GB)
 * 返回值 :
 *      成功返回单位对应的字符串
 *      失败返回"unkown"
 */
char *bi_unit_str(UNIT_TYPE_T unit);

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
int bi_unit_convert(double src, UNIT_TYPE_T src_unit, double *dst, UNIT_TYPE_T *dst_unit);

/* cpu info */
/*
 * 功能 : 获取cpu核心总数
 * 参数 :
 *      cpu_num : cpu核心数量<输出参数>
 * 返回 :
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_num_get(int *cpu_num);

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
int bi_cpu_name_get(int cpu_index, char *cpu_name, int cpu_name_size);

/* 
 * 功能 : 获取cpu频率
 * 参数 :
 *      cpu_index : cpu核心下标
 *      freq : cpu频率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_freq_get(int cpu_index, int *freq);

/* 
 * 功能 : 获取cpu频率
 * 参数 :
 *      cpu_index : cpu核心下标
 *      size : cpu缓存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_cachesize_get(int cpu_index, SIZE_T *size);

/* 
 * 功能 : 获取cpu使用率
 * 参数 :
 *      cpu_index : cpu核心下标
 *      rate : cpu占用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_usagerate_get(int cpu_index, float *rate);

/* 
 * 功能 : 获取cpu总使用率
 * 参数 :
 *      rate : cpu占用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_cpu_total_usagerate_get(float *rate);

/* memory info */
/* 
 * 功能 : 获取内存使用率
 * 参数 :
 *      rate : 内存使用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_usagerate_get(float *rate);

/* 
 * 功能 : 获取内存总量
 * 参数 :
 *      size : 内存总量，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_totalsize_get(SIZE_T *size);

/* 
 * 功能 : 获取可用物理内存大小
 * 参数 :
 *      size : 可用物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_avaliablesize_get(SIZE_T *size);

/* 
 * 功能 : 获取未被使用的物理内存大小
 * 参数 :
 *      size : 未被使用的物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_freesize_get(SIZE_T *size);

/* 
 * 功能 : 获取用于块设备缓冲区的物理内存大小
 * 参数 :
 *      size : 用于块设备缓冲区的物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_buffersize_get(SIZE_T *size);

/* 
 * 功能 : 获取用于用户空间普通文件页的物理内存大小
 * 参数 :
 *      size : 用于用户空间普通文件页的物理内存大小，单位KB<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_mem_cachedsize_get(SIZE_T *size);

/* disk info */
/*
 * 功能 : 更新磁盘信息
 * 参数 :
 *      void
 * 返回 :
 *      成功返回0
 *      失败返回-1
 */
int bi_diskinfo_update(void);

/* 
 * 功能 : 获取磁盘数量
 * 参数 :
 *      disk_num : 磁盘数量<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_num_get(int *disk_num);

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
int bi_disk_name_get(int disk_index, char *name, int name_size);

/* 
 * 功能 : 获取磁盘容量
 * 参数 :
 *      disk_index : 磁盘下标，0 ~ <disk_num - 1>
 *      size : 磁盘容量，单位bytes<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_size_get(int disk_index, SIZE_T *size);

/* 
 * 功能 : 获取磁盘使用率
 * 参数 :
 *      disk_index : 磁盘下标，0 ~ <disk_num - 1>
 *      rate : 磁盘使用率<输出参数>
 * 返回 : 
 *      成功返回0
 *      失败返回-1
 */
int bi_disk_usagerate_get(int disk_index, float *rate);

#endif
