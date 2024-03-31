#ifndef TCP_CLIENT
#define TCP_CLIENT

#include "common.h"

#define TCP_ETHDEV_LEN 16
#define TCP_IPADDR_LEN 32

#define TCP_RECV_DELAY 200
#define TCP_CONN_DELAY 200

#define TCP_RECV_TIMEOUT 100
#define TCP_CONN_TIMEOUT 10

typedef struct 
{
    int sock;
} TCP_CLIENT_DESC_T;

typedef struct 
{
    UINT16_T port;
    char ip[TCP_IPADDR_LEN];
    char ethdev[TCP_ETHDEV_LEN];
} TCP_CLIENT_INFO_T;

typedef struct 
{
    TCP_CLIENT_DESC_T desc;
    TCP_CLIENT_INFO_T info;
} TCP_CLIENT_T;

/* client api */
TCP_CLIENT_T * tcp_client_init(char *ip, UINT16_T port, char *ethdev);

int tcp_client_recv(TCP_CLIENT_T *client, void *data, int data_len);

int tcp_client_send(TCP_CLIENT_T *client, void *data, int data_len);

int tcp_client_uninit(TCP_CLIENT_T *client);

#endif
