#ifndef TCP_SERVER
#define TCP_SERVER

#include "common.h"

#define TCP_ETHDEV_LEN 32
#define TCP_IPADDR_LEN 32

typedef int (*trans_callback_func)(void *, int);

typedef struct 
{
    UINT16_T port;
    char ip[TCP_ETHDEV_LEN];
    char ethdev[TCP_IPADDR_LEN];
} tcp_server_desc_t;

typedef struct 
{
    trans_callback_func recv_callback;
    trans_callback_func send_callback;
} tcp_server_func_t;

typedef struct 
{
    tcp_server_desc_t desc;
    tcp_server_func_t func;
} tcp_server_t;

#endif
