#ifndef THREAD_POOL
#define THREAD_POOL

#include <pthread.h>

#include "sdp_queue.h"

typedef void* (*thread_event_func_t)(void *, int);

typedef enum 
{
    THREAD_POOL_STOP = 0,
    THREAD_POOL_RUNNING,
} THREAD_POOL_STATUS_T;

typedef struct 
{
    thread_event_func_t func; 
    int arg_total_size;
    int arg_used_size;
    char arg[0];
} THREAD_EVENT_T;

typedef struct 
{
    pthread_t pthread;
} THRAED_TASK_T;

typedef struct 
{
    int thread_num;
    int working_num;

    pthread_mutex_t lock;
    pthread_cond_t  notify;

    THRAED_TASK_T  *tasks;
    SDP_QUEUE_T    *event_queue;

    int event_cache_size;
    THREAD_EVENT_T *event_cache;

    THREAD_POOL_STATUS_T status;
} THREAD_POOL_T;

THREAD_POOL_T *thread_pool_create(int thread_num, int event_num, int arg_size);

int thread_pool_destory(THREAD_POOL_T *pool, int force);

int thread_event_add(THREAD_POOL_T *pool, thread_event_func_t func, void *arg, int arg_size);

int thread_event_add_wait(THREAD_POOL_T *pool, thread_event_func_t func, void *arg, int arg_size);

#endif
