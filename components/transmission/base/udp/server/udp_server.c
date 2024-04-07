#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include <sys/select.h>
#include <netinet/udp.h>

#include "udp_server.h"

static int _udp_server_init(UDP_SERVER_T *server)
{
    PTR_CHECK_N1(server);

    int ret = 0;
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

    /* bind udp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, UDP_ETHDEV_LEN, "%s", server->info.ethdev);
    ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        close(sock);
        return -1;
    }

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(server->info.port);  
    inet_pton(AF_INET, server->info.ip, &server_addr.sin_addr);

    /* connect server */
    ret = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        close(sock);
        free(server);
        return NULL;
    }

    server->desc.sock = sock;

    return 0;
}

/* server api */
UDP_SERVER_T * udp_server_init(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    UDP_SERVER_T *server = NULL;

    server = (UDP_SERVER_T *)malloc(sizeof(*server));
    if (!server)
    {
        printf("ERROR : Can not create udp server\n");
        return NULL;
    }

    snprintf(server->info.ip, sizeof(server->info.ip), "%s", ip);
    server->info.port = port;
    snprintf(server->info.ethdev, sizeof(server->info.ethdev), "%s", ethdev);

    if (0 > _udp_server_init(server))
    {
        printf("ERROR : udp server connect failed\n");
        free(server);
        return NULL;
    }

    return server;
}

int udp_server_recv(UDP_SERVER_T *server, void *data, int data_len)
{
    PTR_CHECK_N1(server);
    PTR_CHECK_N1(data);

    int ret    = 0;
    int ready  = 0;
    int recvtm = 0;
    int length = 0;
    int offset = 0;

    fd_set rcvset;
    struct timeval tv;

    tv.tv_sec  = 3;
    tv.tv_usec = 0;
    
    FD_ZERO(&rcvset);
    FD_SET(server->desc.sock, &rcvset);
    
    ready = select(server->desc.sock + 1, &rcvset, NULL, NULL, &tv);
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

    while ((recvtm++ <= UDP_RECV_TIMEOUT))
    {
        length = recv(server->desc.sock, data + offset, data_len - offset, MSG_DONTWAIT);
        if(0 > length)
        {
            if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
            {
                usleep(UDP_RECV_DELAY);
                continue;
            }
            else
            {
                perror("recv error");
                return -1;
            }
        }
        else
        {
            offset += length;

            if (offset >= data_len)
            {
                break;
            }
            
            recvtm = 0;
            continue;
        }
    }

    return offset;
}

int udp_server_send(UDP_SERVER_T *server, void *data, int data_len)
{
    PTR_CHECK_N1(server);
    PTR_CHECK_N1(data);

    int ret    = 0;
    int length = 0;
    int socket = server->desc.sock;

    length = send(socket, data, data_len, MSG_DONTWAIT);
    if (0 > length)
    {
        perror("write error");
    }

    return length;
}

int udp_server_uninit(UDP_SERVER_T *server)
{
    PTR_CHECK_N1(server);

    close(server->desc.sock);

    free(server);

    return 0;
}
