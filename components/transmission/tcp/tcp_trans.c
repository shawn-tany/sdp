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

/* client api */

static int tcp_client_init(MFTP_TCP_DESC_T *tcp_desc)
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
    ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect error");
        close(sock);
        return -1;
    }

    tcp_desc->sock = sock;

    return 0;
}

static int tcp_re_connect(MFTP_TCP_DESC_T *tcp_desc)
{
    struct tcp_info info;
    int ret    = 0;
    int conntm = 0;
    int len    = sizeof(info);
    int sock   = tcp_desc->sock;

    ret = getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)(&len));
    if (0 > ret)
    {
        perror("getsockopt error");
        return -1;
    }

    if (TCP_ESTABLISHED != info.tcpi_state)
    {
        while (conntm++ <= MFTP_TCP_CONN_TIMEOUT)
        {
            printf("try to re-connect times(%d)\n", conntm);

            close(tcp_desc->sock);

            if (0 > tcp_client_init(tcp_desc))
            {
                usleep(MFTP_TCP_CONN_DELAY);
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

static int tcp_client_recv(MFTP_TCP_DESC_T *tcp_desc, MFTP_MSG_T *mftp_msg)
{
    PTR_CHECK_N1(tcp_desc);
    PTR_CHECK_N1(mftp_msg);

    int ret     = 0;
    int ready   = 0;
    int recvtm  = 0;
    int length  = 0;

    fd_set rcvset;
    struct timeval tv;

    tv.tv_sec  = 3;
    tv.tv_usec = 0;

    if ((ret = tcp_re_connect(tcp_desc)))
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
    FD_SET(tcp_desc->sock, &rcvset);
    
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

    mftp_msg->data_len = 0;

    while ((recvtm++ <= MFTP_TCP_RECV_TIMEOUT))
    {
        length = recv(ready, (mftp_msg->data + mftp_msg->data_len), (mftp_msg->data_size - mftp_msg->data_len), MSG_DONTWAIT);
        if(0 > length)
        {
            if ((EAGAIN == errno) || (EWOULDBLOCK == errno))
            {
                usleep(MFTP_TCP_RECV_DELAY);
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
            if ((ret = tcp_re_connect(tcp_desc)))
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
            mftp_msg->data_len += length;

            if (mftp_msg->data_len >= sizeof(MFTP_MSG_HEADER_T))
            {
                mftp_msg->msg_header = (MFTP_MSG_HEADER_T *)(mftp_msg->data);

                if (MFTP_MSG_TYPE_MSG_INFO == mftp_msg->msg_header->msg_type &&
                    mftp_msg->data_len >= (sizeof(MFTP_MSG_HEADER_T) + sizeof(MFTP_MSG_INFO_T)))
                {
                    mftp_msg->msg_info = (MFTP_MSG_INFO_T *)(mftp_msg->data + sizeof(MFTP_MSG_HEADER_T));

                    if (mftp_msg->msg_info->totol_data_size <= mftp_msg->data_len)
                    {
                        break;
                    }
                }
            }
            
            recvtm = 0;
            continue;
        }
    }

    return mftp_msg->data_len;
}

static int tcp_client_send(MFTP_TCP_DESC_T *tcp_desc, MFTP_MSG_T *mftp_msg)
{
    PTR_CHECK_N1(tcp_desc);
    PTR_CHECK_N1(mftp_msg);

    int ret    = 0;
    int length = 0;
    int socket = tcp_desc->sock;

    /* re-connect */
    if ((ret = tcp_re_connect(tcp_desc)))
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

    length = send(socket, mftp_msg->data, mftp_msg->data_len, MSG_DONTWAIT);
    if (0 > length)
    {
        perror("write error");
    }

    return length;
}


int tcp_init(void *desc)
{
    PTR_CHECK_N1(desc);

    int ret = 0;
    MFTP_TCP_DESC_T *tcp_desc = desc;

    if (tcp_desc->is_server)
    {
        ret = tcp_server_init(tcp_desc);
    }
    else
    {
        ret = tcp_client_init(tcp_desc);
    }

    return ret;
}

int tcp_recv(void *desc, void *msg)
{
    PTR_CHECK_N1(desc);
    PTR_CHECK_N1(msg);

    int ret = 0;
    MFTP_TCP_DESC_T *tcp_desc = desc;
    MFTP_MSG_T *mftp_msg = msg;

    if (tcp_desc->is_server)
    {
        ret = tcp_server_trans(tcp_desc, mftp_msg, MFTP_DIRECTION_RECV);
    }
    else
    {
        ret = tcp_client_recv(tcp_desc, mftp_msg);
    }

    return ret;
}

int tcp_send(void *desc, void *msg)
{
    PTR_CHECK_N1(desc);
    PTR_CHECK_N1(msg);

    int ret = 0;
    MFTP_TCP_DESC_T *tcp_desc = desc;
    MFTP_MSG_T *mftp_msg = msg;

    if (tcp_desc->is_server)
    {
        ret = tcp_server_trans(tcp_desc, mftp_msg, MFTP_DIRECTION_SEND);
    }
    else
    {
        ret = tcp_client_send(tcp_desc, mftp_msg);
    }

    return ret;
}

int tcp_uninit(void *desc)
{
    PTR_CHECK_N1(desc);

    MFTP_TCP_DESC_T *tcp_desc = desc;

    close(tcp_desc->sock);

    return 0;
}

static MFTP_TRANS_HANDLE_T tcp_handle = {
    .trans_init = tcp_init,
    .trans_recv = tcp_recv,
    .trans_send = tcp_send,
    .trans_uninit = tcp_uninit
};

MFTP_TRANS_HANDLE_REG_CONSTRUCTOR(MFTP_TRANS_TYPE_TCP, tcp_handle);
