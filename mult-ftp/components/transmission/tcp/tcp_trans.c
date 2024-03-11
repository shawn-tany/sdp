#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h> 
#include <net/if.h>
#include <netinet/tcp.h>

#include "tcp_trans.h"
#include "msg.h"
#include "transmission.h"

static int tcp_connect(MFTP_TCP_DESC_T *mftp_tcp_desc)
{
    PTR_CHECK_N1(mftp_tcp_desc);

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
    snprintf(ifrq.ifr_ifrn.ifrn_name, MFTP_TCP_ETHDEV_LEN, "%s", mftp_tcp_desc->ethdev);
    ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        close(sock);
        return -1;
    }

    /* server ip & server port */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(mftp_tcp_desc->port);  
    inet_pton(AF_INET, mftp_tcp_desc->ip, &server_addr.sin_addr);

    /* connect server */
    ret = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        perror("connect error");
        close(sock);
        return -1;
    }

    mftp_tcp_desc->socket = sock;

    return 0;
}

static int tcp_re_connect(MFTP_TCP_DESC_T *mftp_tcp_desc)
{
    struct tcp_info info;
    int ret    = 0;
    int conntm = 0;
    int len    = sizeof(info);
    int sock   = mftp_tcp_desc->socket;

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

            close(mftp_tcp_desc->socket);

            if (0 > tcp_connect(mftp_tcp_desc))
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

int tcp_init(void *tcp_desc)
{
    PTR_CHECK_N1(tcp_desc);
    
    MFTP_TCP_DESC_T *mftp_tcp_desc = tcp_desc;

    if (0 > tcp_connect(mftp_tcp_desc))
    {
        return -1;
    }

    return 0;
}

int tcp_recv(void *tcp_desc, void *msg)
{
    PTR_CHECK_N1(tcp_desc);
    PTR_CHECK_N1(msg);

    MFTP_TCP_DESC_T *mftp_tcp_desc = tcp_desc;
    MFTP_MSG_TRANS_T *mftp_msg = msg;

    int ret     = 0;
    int ready   = 0;
    int recvtm  = 0;
    int length  = 0;
    int socket  = mftp_tcp_desc->socket;

    fd_set rcvset;
    struct timeval tv;

    tv.tv_sec  = 3;
    tv.tv_usec = 0;

    FD_ZERO(&rcvset);
    FD_SET(socket, &rcvset);

    if ((ret = tcp_re_connect(mftp_tcp_desc)))
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
    
    ready = select(socket + 1, &rcvset, NULL, NULL, &tv);
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
        length = recv(socket, (mftp_msg->data + mftp_msg->data_len), (mftp_msg->data_size - mftp_msg->data_len), MSG_DONTWAIT);
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
            if ((ret = tcp_re_connect(mftp_tcp_desc)))
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
            recvtm = 0;
            continue;
        }
    }

    return mftp_msg->data_len;
}

int tcp_send(void *tcp_desc, void *msg)
{
    PTR_CHECK_N1(tcp_desc);
    PTR_CHECK_N1(msg);

    MFTP_TCP_DESC_T *mftp_tcp_desc = tcp_desc;
    MFTP_MSG_TRANS_T *mftp_msg = msg;

    int ret    = 0;
    int length = 0;
    int socket = mftp_tcp_desc->socket;

    /* re-connect */
    if ((ret = tcp_re_connect(mftp_tcp_desc)))
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


int tcp_uninit(void *tcp_desc)
{
    PTR_CHECK_N1(tcp_desc);

    return 0;
}

static MFTP_TRANS_HANDLE_T tcp_handle = {
    .trans_init = tcp_init,
    .trans_recv = tcp_recv,
    .trans_send = tcp_send,
    .trans_uninit = tcp_uninit
};

MFTP_TRANS_HANDLE_REG_CONSTRUCTOR(MFTP_TRANS_TYPE_TCP, tcp_handle);
