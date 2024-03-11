#ifndef MFTP_TCP_TRANSMISSION
#define MFTP_TCP_TRANSMISSION

#include "common.h"

#define MFTP_TCP_ETHDEV_LEN 32
#define MFTP_TCP_IPADDR_LEN 32
#define MFTP_TCP_RECV_DELAY 200
#define MFTP_TCP_CONN_DELAY 200
#define MFTP_TCP_RECV_TIMEOUT 100
#define MFTP_TCP_CONN_TIMEOUT 10

typedef struct 
{
    int socket;
    UINT16_T port;
    char ip[MFTP_TCP_ETHDEV_LEN];
    char ethdev[MFTP_TCP_IPADDR_LEN];
} MFTP_TCP_DESC_T;

int tcp_init();

int tcp_recv();

int tcp_send();

int tcp_uninit();

#endif
