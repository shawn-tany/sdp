#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <pthread.h>

#include "tcp_server.h"

/* server api */

TCP_SERVER_T *tcp_server_init(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    int ret  = 0;
    struct sockaddr_in server_addr = {0};
    struct ifreq ifrq = {0};
    TCP_SERVER_T *server = NULL;

    server = (TCP_SERVER_T *)malloc(sizeof(*server));
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
        close(server->desc.socket);
        free(server);
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
        close(server->desc.socket);
        free(server);
        return NULL;
    }

    listen(server->desc.socket, TCP_MAX_CLIENT);

    /* tcp server thread pool */
    server->work.pool = thread_pool_create(TCP_MAX_CLIENT, TCP_MAX_EVENT);
    if (!server->work.pool)
    {
        printf("failed to create tcp server thread pool\n");
        close(server->desc.socket);
        free(server);
        return NULL;
    }

    return server;
}

static void *tcp_server_task_loop(void *arg, int arg_size)
{
    PTR_CHECK_NULL(arg);

    char w_buffer[1024] = {0};
    char r_buffer[1024] = {0};
    int length = 0;
    TCP_SERVER_TASK_T *task = (TCP_SERVER_TASK_T *)arg;

    while (1)
    {
        length = recv(task->socket, r_buffer, sizeof(r_buffer), 0);
        if (0 > length)
        {
            perror("recv");
            break;
        }
        else if (0 == length)
        {
            break;
        }
        else
        {
            task->func.recv_complete_callback(r_buffer, length);
        }

        task->func.send_encap_callback(w_buffer, sizeof(w_buffer), &length);

        if (0 > send(task->socket, w_buffer, length, 0))
        {
            perror("send");
            break;
        }
    }

    close(task->socket);

    return NULL;
}

int tcp_server_loop(TCP_SERVER_T *server, TCP_SERVER_FUNC_T *func_opts)
{
    PTR_CHECK_N1(server);
    PTR_CHECK_N1(func_opts);

    int sock = 0;

    struct sockaddr_in server_addr = {0};
    socklen_t client_len = {0};
    TCP_SERVER_TASK_T task;

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
            thread_pool_destory(server->work.pool, 1);
            return -1;
        }

        task.socket = sock;
        task.func   = *func_opts;

        if (0 > thread_event_add(server->work.pool, tcp_server_task_loop, (void *)(&task), sizeof(task)))
        {
            printf("add tcp client event to queue failed\n");
            break;
        }

        server->work.work_count++;
    }
    
    return 0;
}

