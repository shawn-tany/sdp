#ifndef UDP_SERVER
#define UDP_SERVER

#include "common.h"

#define UDP_ETHDEV_LEN 16
#define UDP_IPADDR_LEN 32

#define UDP_RECV_DELAY 200
#define UDP_CONN_DELAY 200

#define UDP_RECV_TIMEOUT 100
#define UDP_CONN_TIMEOUT 10

typedef struct 
{
    int sock;
} UDP_SERVER_DESC_T;

typedef struct 
{
    UINT16_T port;
    char ip[UDP_IPADDR_LEN];
    char ethdev[UDP_ETHDEV_LEN];
} UDP_SERVER_INFO_T;

typedef struct 
{
    UDP_SERVER_DESC_T desc;
    UDP_SERVER_INFO_T info;
} UDP_SERVER_T;

/* client api */
UDP_SERVER_T * udp_server_init(char *ip, UINT16_T port, char *ethdev);

int udp_server_recv(UDP_SERVER_T *client, void *data, int data_len);

int udp_server_send(UDP_SERVER_T *client, void *data, int data_len);

int udp_server_uninit(UDP_SERVER_T *client);

#endif
