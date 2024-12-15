#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/select.h>
#include <netinet/udp.h>

#include "udp.h"

static int _udp_trans_init(UDP_TRANS_T *udp_trans)
{
    PTR_CHECK_N1(udp_trans);

    int ret = 0;
    int sock = 0;
    struct ifreq ifrq = {0};
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    /* create socket */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (0 > sock) 
    {
        perror("socket error");
        return -1;
    }

    /* bind udp_ctl->ethdev */
    snprintf(ifrq.ifr_ifrn.ifrn_name, UDP_ETHDEV_LEN, "%s", udp_trans->info.ethdev);
    ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (char *)&ifrq, sizeof(ifrq));
    if (ret < 0)
    {
        perror("setsockopt error");
        close(sock);
        return -1;
    }

    /* server ip & server port */
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(udp_trans->info.port);  
    inet_pton(AF_INET, udp_trans->info.ip, &(addr.sin_addr));

    if (0 > bind(sock, (struct sockaddr *)&addr, addrlen))
    {
        perror("bind error");
        close(sock);
        return -1;
    }

    udp_trans->desc.sock = sock;

    return 0;
}

/* udp_trans api */
UDP_TRANS_T * udp_trans_init(char *ip, UINT16_T port, char *ethdev)
{
    PTR_CHECK_NULL(ip);
    PTR_CHECK_NULL(ethdev);

    UDP_TRANS_T *udp_trans = NULL;

    udp_trans = (UDP_TRANS_T *)malloc(sizeof(*udp_trans));
    if (!udp_trans)
    {
        printf("ERROR : Can not create udp trans\n");
        return NULL;
    }

    snprintf(udp_trans->info.ip, sizeof(udp_trans->info.ip), "%s", ip);
    udp_trans->info.port = port;
    snprintf(udp_trans->info.ethdev, sizeof(udp_trans->info.ethdev), "%s", ethdev);

    if (0 > _udp_trans_init(udp_trans))
    {
        printf("ERROR : udp trans init failed\n");
        free(udp_trans);
        return NULL;
    }

    return udp_trans;
}

int udp_trans_recv(UDP_TRANS_T *udp_trans, char *src_ip, UINT16_T *src_port, void *data, int data_len)
{
    PTR_CHECK_N1(udp_trans);
    PTR_CHECK_N1(data);

    int ready  = 0;
    int length = 0;
    int total_len = 0;

    fd_set rcvset;
    struct timeval tv;

    struct sockaddr_in src_addr;
    socklen_t src_addrlen = sizeof(src_addr);

    tv.tv_sec  = 5;
    tv.tv_usec = 0;
    
    FD_ZERO(&rcvset);
    FD_SET(udp_trans->desc.sock, &rcvset);
    
    ready = select(udp_trans->desc.sock + 1, &rcvset, NULL, NULL, &tv);
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

    do
    {
        length = recvfrom(udp_trans->desc.sock, data + total_len, data_len - total_len, MSG_DONTWAIT, (struct  sockaddr *)&src_addr, &src_addrlen);
        if(0 > length)
        {
            perror("recv error");
            return -1;
        }
        else
        {
            total_len += length;
        }
    } while ((total_len < data_len) & length);

    *src_port = ntohs(src_addr.sin_port);
    inet_ntop(AF_INET, (void *)&(src_addr.sin_addr), src_ip, UDP_IPADDR_LEN);

    return total_len;
}

int udp_trans_send(UDP_TRANS_T *udp_trans, const char *dst_ip, UINT16_T dst_port, void *data, int data_len)
{
    PTR_CHECK_N1(udp_trans);
    PTR_CHECK_N1(data);

    int length = 0;
    int socket = udp_trans->desc.sock;

    struct sockaddr_in dst_addr;
    socklen_t dst_addrlen = sizeof(dst_addr);

    memset(&dst_addr, 0, sizeof(dst_addr));
    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port   = htons(dst_port);  
    inet_pton(AF_INET, dst_ip, &(dst_addr.sin_addr));

    length = sendto(socket, data, data_len, MSG_DONTWAIT, (struct sockaddr *)&dst_addr, dst_addrlen);
    if (0 > length)
    {
        perror("write error");
    }

    return length;
}

int udp_trans_uninit(UDP_TRANS_T *udp_trans)
{
    PTR_CHECK_N1(udp_trans);

    close(udp_trans->desc.sock);

    free(udp_trans);

    return 0;
}
