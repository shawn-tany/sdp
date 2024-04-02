#ifndef TCP_SERVER
#define TCP_SERVER

#include "common.h"
#include "thread_pool.h"

#define TCP_ETHDEV_LEN 16
#define TCP_IPADDR_LEN 32
#define TCP_MAX_CLIENT 10
#define TCP_MAX_EVENT  120

typedef int (*recv_complete_callback_func_t)(void *, int);
typedef int (*send_encap_callback_func_t)(void *, int, int *);

typedef struct 
{
    int socket;
} TCP_SERVER_DESC_T;

typedef struct 
{
    UINT16_T port;
    char ip[TCP_IPADDR_LEN];
    char ethdev[TCP_ETHDEV_LEN];
} TCP_SERVER_INFO_T;

typedef struct 
{
    UINT16_T work_count;
    UINT16_T quit;
    THREAD_POOL_T *pool;
} TCP_SERVER_WORK_T;

typedef struct 
{
    TCP_SERVER_DESC_T desc;
    TCP_SERVER_INFO_T info;
    TCP_SERVER_WORK_T work;
} TCP_SERVER_T;

typedef struct 
{
    recv_complete_callback_func_t recv_complete_callback;
    send_encap_callback_func_t send_encap_callback;
} TCP_SERVER_FUNC_T;

typedef struct 
{
    int socket;
    TCP_SERVER_FUNC_T func;
} TCP_SERVER_TASK_T;

TCP_SERVER_T *tcp_server_init(char *ip, UINT16_T port, char *ethdev);

int tcp_server_loop(TCP_SERVER_T *server, TCP_SERVER_FUNC_T *func_opts);

int tcp_server_destory(TCP_SERVER_T *server);

#endif
