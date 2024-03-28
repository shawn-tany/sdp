#ifndef THREAD_POOL
#define THREAD_POOL

#define THREAD_MAX_NUM 32

typedef struct 
{
    
} THRAD_CHANNEL_T;

typedef struct 
{
    int thread_num;
    THRAD_CHANNEL_T cahnnel[THREAD_MAX_NUM];
} THREAD_POOL_T;

#endif