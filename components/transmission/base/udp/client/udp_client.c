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

#include "udp_client.h"

static int _udp_client_init(UDP_CLIENT_T *client)
{
    PTR_CHECK_N1(client);

    int ret = 0;
    int sock = 0;
    struct ifreq ifrq = {0};

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > sock) 
    {
        perror("socket error");
        return -1;
    }

    /* bind udp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, UDP_ETHDEV_LEN, "%s", client->info.ethdev);
    ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        close(sock);
        return -1;
    }

    /* server ip & server port */
    client->desc.server_addr.sin_family = AF_INET;
    client->desc.server_addr.sin_port   = htons(client->info.port);  
    inet_pton(AF_INET, client->info.ip, &(client->desc.server_addr.sin_addr));

    client->desc.sock = sock;

    return 0;
}

/* client api */
UDP_CLIENT_T * udp_client_init(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    UDP_CLIENT_T *client = NULL;

    client = (UDP_CLIENT_T *)malloc(sizeof(*client));
    if (!client)
    {
        printf("ERROR : Can not create udp client\n");
        return NULL;
    }

    snprintf(client->info.ip, sizeof(client->info.ip), "%s", ip);
    client->info.port = port;
    snprintf(client->info.ethdev, sizeof(client->info.ethdev), "%s", ethdev);

    if (0 > _udp_client_init(client))
    {
        printf("ERROR : udp client connect failed\n");
        free(client);
        return NULL;
    }

    return client;
}

int udp_client_recv(UDP_CLIENT_T *client, void *data, int data_len)
{
    PTR_CHECK_N1(client);
    PTR_CHECK_N1(data);

    int ret    = 0;
    int ready  = 0;
    int recvtm = 0;
    int length = 0;
    int total_len = 0;

    fd_set rcvset;
    struct timeval tv;

    tv.tv_sec  = 3;
    tv.tv_usec = 0;
    
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

    while ((total_len < data_len))
    {
        length = recv(client->desc.sock, data + total_len, data_len - total_len, MSG_DONTWAIT);
        if(0 > length)
        {
            perror("recv error");
            return -1;
        }
        else
        {
            total_len += length;
        }
    }

    return total_len;
}

int udp_client_send(UDP_CLIENT_T *client, void *data, int data_len)
{
    PTR_CHECK_N1(client);
    PTR_CHECK_N1(data);

    int ret    = 0;
    int length = 0;
    int socket = client->desc.sock;

    length = send(socket, data, data_len, MSG_DONTWAIT);
    if (0 > length)
    {
        perror("write error");
    }

    return length;
}

int udp_client_uninit(UDP_CLIENT_T *client)
{
    PTR_CHECK_N1(client);

    close(client->desc.sock);

    free(client);

    return 0;
}
