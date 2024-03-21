#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include <sys/select.h>
#include <netinet/tcp.h>

#include "tcp_trans.h"
#include "msg.h"
#include "transmission.h"

/* server api */

static int tcp_server_init(MFTP_TCP_DESC_T *tcp_desc)
{
    PTR_CHECK_N1(tcp_desc);

    int ret  = 0;
    int sock = 0;
    struct sockaddr_in server_addr = {0};
    struct ifreq ifrq = {0};

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sock) 
    {
        perror("socket error");
        return -1;
    }

    /* bind tcp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, MFTP_TCP_ETHDEV_LEN, "%s", tcp_desc->ethdev);
    ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        close(sock);
        return -1;
    }

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(tcp_desc->port);
    inet_pton(AF_INET, tcp_desc->ip, &server_addr.sin_addr);

    /* connect server */
    ret = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        close(sock);
        return -1;
    }

    listen(sock, MFTP_TCP_MAX_CLIENT);

    tcp_desc->sock = sock;

    return 0;
}

static int tcp_server_trans(MFTP_TCP_DESC_T *tcp_desc, MFTP_MSG_T *msg, MFTP_DIRECTION_T direction)
{
    PTR_CHECK_N1(tcp_desc);

    int i = 0;
    int sock = 0;
    int num  = 0;
    int ready = 0;

    struct sockaddr_in server_addr = {0};
    socklen_t client_len = {0};

    fd_set rcvset;
    struct timeval tv;

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(tcp_desc->port);
    inet_pton(AF_INET, tcp_desc->ip, &server_addr.sin_addr);

    FD_ZERO(&rcvset);
    FD_SET(tcp_desc->sock, &rcvset);

    tv.tv_sec  = 3;
    tv.tv_usec = 0;
    
    ready = select(tcp_desc->sock + 1, &rcvset, NULL, NULL, &tv);
    if (0 > ready)
    {
        perror("select error");
        return -1;
    }
    else if (0 == ready)
    {
        printf("Select timeout\n");
        return -1;
    }

    if (FD_ISSET(tcp_desc->sock, &rcvset))
    {
        sock = accept(tcp_desc->sock + 1, (struct sockaddr *)&server_addr, &client_len);
        if (0 > sock) 
        {
            perror("accept error");
            return -1;
        }

        for (i = 0; i < ITEM(tcp_desc->client_sock); ++i)
        {
            tcp_desc->client_sock[num] = sock;
        }
    }

    UNUSED(msg);
    
    return sock;
}

