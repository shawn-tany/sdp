#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include <sys/select.h>
#include <netinet/tcp.h>

#include "tcp_server.h"

/* server api */

tcp_server_acceptor_t *tcp_server_acceptor(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    int ret  = 0;
    struct sockaddr_in server_addr = {0};
    struct ifreq ifrq = {0};
    tcp_server_acceptor_t *acceptor = NULL;

    acceptor = (tcp_server_acceptor_t *)malloc(sizeof(*acceptor));
    if (!acceptor)
    {
        printf("failed to create tcp server acceptor\n");
        return NULL;
    }

    snprintf(acceptor->info.ip, sizeof(acceptor->info.ip), "%s", ip);
    acceptor->info.port = port;
    snprintf(acceptor->info.ethdev, sizeof(acceptor->info.ethdev), "%s", ethdev);

    /* create socket */
    acceptor->desc.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > acceptor->desc.socket) 
    {
        perror("socket error");
        free(acceptor);
        return NULL;
    }

    /* bind tcp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, sizeof(ifrq.ifr_ifrn.ifrn_name), "%s", acceptor->info.ethdev);
    ret = setsockopt(acceptor->desc.socket, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        free(acceptor);
        close(acceptor->desc.socket);
        return NULL;
    }

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(acceptor->info.port);
    inet_pton(AF_INET, acceptor->info.ip, &server_addr.sin_addr);

    /* connect server */
    ret = bind(acceptor->desc.socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        free(acceptor);
        close(acceptor->desc.socket);
        return NULL;
    }

    listen(acceptor->desc.socket, TCP_MAX_CLIENT);

    return acceptor;
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

