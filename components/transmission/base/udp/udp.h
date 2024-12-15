#ifndef UDP_SERVER
#define UDP_SERVER

#include <arpa/inet.h> 

#include "common.h"

#define UDP_ETHDEV_LEN 16
#define UDP_IPADDR_LEN 16

#define UDP_RECV_DELAY 200
#define UDP_CONN_DELAY 200

#define UDP_RECV_TIMEOUT 100
#define UDP_CONN_TIMEOUT 10

typedef struct 
{
    int sock;
} UDP_TRANS_DESC_T;

typedef struct 
{
    UINT16_T port;
    char ip[UDP_IPADDR_LEN];
    char ethdev[UDP_ETHDEV_LEN];
} UDP_TRANS_INFO_T;

typedef struct 
{
    UDP_TRANS_DESC_T desc;
    UDP_TRANS_INFO_T info;
} UDP_TRANS_T;

/* client api */
UDP_TRANS_T * udp_trans_init(char *ip, UINT16_T port, char *ethdev);

int udp_trans_recv(UDP_TRANS_T *udp_trans, char *src_ip, UINT16_T *src_port, void *data, int data_len);

int udp_trans_send(UDP_TRANS_T *udp_trans, const char *dst_ip, UINT16_T dst_port, void *data, int data_len);

int udp_trans_uninit(UDP_TRANS_T *client);

#endif
