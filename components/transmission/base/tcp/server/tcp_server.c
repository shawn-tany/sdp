#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <pthread.h>

#include "tcp_server.h"

/* server api */

tcp_server_t *tcp_server_init(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    int ret  = 0;
    struct sockaddr_in server_addr = {0};
    struct ifreq ifrq = {0};
    tcp_server_t *server = NULL;

    server = (tcp_server_t *)malloc(sizeof(*server));
    if (!server)
    {
        printf("failed to create tcp server\n");
        return NULL;
    }
    memset(server, 0, sizeof(*server));

    snprintf(server->info.ip, sizeof(server->info.ip), "%s", ip);
    server->info.port = port;
    snprintf(server->info.ethdev, sizeof(server->info.ethdev), "%s", ethdev);

    /* create socket */
    server->desc.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > server->desc.socket) 
    {
        perror("socket error");
        free(server);
        return NULL;
    }

    /* bind tcp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, sizeof(ifrq.ifr_ifrn.ifrn_name), "%s", server->info.ethdev);
    ret = setsockopt(server->desc.socket, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        free(server);
        close(server->desc.socket);
        return NULL;
    }

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(server->info.port);
    inet_pton(AF_INET, server->info.ip, &server_addr.sin_addr);

    /* connect server */
    ret = bind(server->desc.socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("bind error");
        free(server);
        close(server->desc.socket);
        return NULL;
    }

    listen(server->desc.socket, TCP_MAX_CLIENT);

    return server;
}

static tcp_server_channel_t *tcp_server_channel_create(tcp_server_func_t *func_opts)
{
    PTR_CHECK_N1(func_opts);

    tcp_server_channel_t *channel = NULL;

    channel = (tcp_server_channel_t *)malloc(sizeof(tcp_server_channel_t));
    if (!channel)
    {
        printf("failed to create tcp server channel\n");
        return NULL;
    }

    channel->func = *func_opts;

    return channel;
}

static void *tcp_server_channel_loop(void *arg)
{
    return NULL;
}

static void tcp_server_channel_destory(tcp_server_channel_t *channel)
{
    PTR_CHECK_VOID(channel);

    free(channel);
}

int tcp_server_loop(tcp_server_t *server, tcp_server_func_t *func_opts)
{
    PTR_CHECK_N1(server);
    PTR_CHECK_N1(func_opts);

    int sock = 0;

    struct sockaddr_in server_addr = {0};
    socklen_t client_len = {0};
    tcp_server_channel_t *channel = NULL;

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(server->info.port);
    inet_pton(AF_INET, server->info.ip, &server_addr.sin_addr);

    while (!(server->work.quit))
    {
        if (TCP_MAX_CLIENT <= server->work.work_count)
        {
            usleep(2000);
            continue;
        }

        sock = accept(server->desc.socket, (struct sockaddr *)&server_addr, &client_len);
        if (0 > sock) 
        {
            perror("accept error");
            return -1;
        }

        channel = tcp_server_channel_create(func_opts);
        if (!channel)
        {
            printf("failed to create tcp server cahnnel\n");
            return -1;
        }

        channel->socket = sock;

        if (0 > pthread_create(&channel->pthread, NULL, tcp_server_channel_loop, (void *)channel))
        {
            perror("pthread create failed");
            return -1;
        }

        server->work.work_count++;
    }
    
    return 0;
}

