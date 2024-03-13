#ifndef MFTP_TCP_TRANSMISSION
#define MFTP_TCP_TRANSMISSION

#include "common.h"
#include "msg.h"

#define MFTP_TCP_ETHDEV_LEN 32
#define MFTP_TCP_IPADDR_LEN 32

#define MFTP_TCP_RECV_DELAY 200
#define MFTP_TCP_CONN_DELAY 200

#define MFTP_TCP_RECV_TIMEOUT 100
#define MFTP_TCP_CONN_TIMEOUT 10

#define MFTP_TCP_MAX_CLIENT 10

typedef struct 
{
    UINT16_T is_server : 1;
    UINT16_T rsv : 15;
    UINT16_T port;
    char ip[MFTP_TCP_ETHDEV_LEN];
    char ethdev[MFTP_TCP_IPADDR_LEN];

    int sock;    

    /* server accept socket */
    int client_sock[MFTP_TCP_MAX_CLIENT];
} MFTP_TCP_DESC_T;

int tcp_init();

int tcp_uninit();

int tcp_recv();

int tcp_send();

#endif
