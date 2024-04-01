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
#include <netinet/tcp.h>

#include "tcp_client.h"

static int _tcp_client_init(TCP_CLIENT_T *client)
{
    PTR_CHECK_N1(client);

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

    /* bind tcp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, TCP_ETHDEV_LEN, "%s", client->info.ethdev);
    ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        close(sock);
        return -1;
    }

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(client->info.port);  
    inet_pton(AF_INET, client->info.ip, &server_addr.sin_addr);

    /* connect server */
    ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect error");
        close(sock);
        return -1;
    }

    client->desc.sock = sock;

    return 0;
}

static int _tcp_client_reconnect(TCP_CLIENT_T *client)
{
    PTR_CHECK_N1(client);

    struct tcp_info info;
    int ret    = 0;
    int conntm = 0;
    int len    = sizeof(info);
    int sock   = client->desc.sock;

    ret = getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)(&len));
    if (0 > ret)
    {
        perror("getsockopt error");
        return -1;
    }

    if (TCP_ESTABLISHED != info.tcpi_state)
    {
        while (conntm++ <= TCP_CONN_TIMEOUT)
        {
            printf("try to re-connect times(%d)\n", conntm);

            close(client->desc.sock);

            if (0 > _tcp_client_init(client))
            {
                usleep(TCP_CONN_DELAY);
            }
            else
            {
                return 1;
            }
        }
    }
    else
    {
        return 0;
    }

    return -1;
}

/* client api */
TCP_CLIENT_T * tcp_client_init(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    TCP_CLIENT_T *client = NULL;

    client = (TCP_CLIENT_T *)malloc(sizeof(*client));
    if (!client)
    {
        printf("ERROR : Can not create tcp client\n");
        return NULL;
    }

    snprintf(client->info.ip, sizeof(client->info.ip), "%s", ip);
    client->info.port = port;
    snprintf(client->info.ethdev, sizeof(client->info.ethdev), "%s", ethdev);

    if (0 > _tcp_client_init(client))
    {
        printf("ERROR : tcp client connect failed\n");
        free(client);
        return NULL;
    }

    return client;
}

int tcp_client_recv(TCP_CLIENT_T *client, void *data, int data_len)
{
    PTR_CHECK_N1(client);
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

    if ((ret = _tcp_client_reconnect(client)))
    {
        if (-1 == ret)
        {
            printf("tcp re-connect before recv error\n");
            return -1;
        }
        else
        {
            printf("tcp re-connect before recv success\n");
        }
    }
    
    FD_ZERO(&rcvset);
    FD_SET(client->desc.sock, &rcvset);
    
    ready = select(client->desc.sock + 1, &rcvset, NULL, NULL, &tv);
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

    while ((recvtm++ <= TCP_RECV_TIMEOUT))
    {
        length = recv(ready, data + offset, data_len - offset, MSG_DONTWAIT);
        if(0 > length)
        {
            if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
            {
                usleep(TCP_RECV_DELAY);
                continue;
            }
            else
            {
                perror("recv error");
                return -1;
            }
        }
        else if(length == 0)
        {
            if ((ret = _tcp_client_reconnect(client)))
            {
                if (-1 == ret)
                {
                    printf("tcp re-connect after recv error\n");
                    return -1;
                }
                else
                {
                    printf("tcp re-connect after recv success\n");
                    recvtm = 0;
                    continue;
                }
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

int tcp_client_send(TCP_CLIENT_T *client, void *data, int data_len)
{
    PTR_CHECK_N1(client);
    PTR_CHECK_N1(data);

    int ret    = 0;
    int length = 0;
    int socket = client->desc.sock;

    /* re-connect */
    if ((ret = _tcp_client_reconnect(client)))
    {
        if (-1 == ret)
        {
            printf("tcp re-connect error\n");
            return -1;
        }
        else
        {
            printf("tcp re-connect success\n");
        }
    }

    length = send(socket, data, data_len, MSG_DONTWAIT);
    if (0 > length)
    {
        perror("write error");
    }

    return length;
}

int tcp_client_uninit(TCP_CLIENT_T *client)
{
    PTR_CHECK_N1(client);

    close(client->desc.sock);

    free(client);

    return 0;
}
