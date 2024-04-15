#ifndef UDP_CLIENT
#define UDP_CLIENT

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
    struct sockaddr_in server_addr;
} UDP_CLIENT_DESC_T;

typedef struct 
{
    UINT16_T port;
    char ip[UDP_IPADDR_LEN];
    char ethdev[UDP_ETHDEV_LEN];
} UDP_CLIENT_INFO_T;

typedef struct 
{
    UDP_CLIENT_DESC_T desc;
    UDP_CLIENT_INFO_T info;
} UDP_CLIENT_T;

/* client api */
UDP_CLIENT_T * udp_client_init(char *ip, UINT16_T port, char *ethdev);

int udp_client_recv(UDP_CLIENT_T *client, void *data, int data_len);

int udp_client_send(UDP_CLIENT_T *client, void *data, int data_len);

int udp_client_uninit(UDP_CLIENT_T *client);

#endif
