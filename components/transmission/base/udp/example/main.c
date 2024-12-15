#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

#include "udp.h"

static struct option longopts[] = {
    {"src_ip",   required_argument, 0, 0x0010},
    {"src_port", required_argument, 0, 0x0011},
    {"dst_ip",   required_argument, 0, 0x0012},
    {"dst_port", required_argument, 0, 0x0013}
};

static void option_help(char *program)
{
    printf("%s Option :\n"
            "   --src_ip   <src ip>\n"
            "   --src_port <src port>\n"
            "   --dst_ip   <dst ip>\n"
            "   --dst_port <dst port>\n",
            program);
}

int main(int argc, char *argv[])
{
    UDP_TRANS_T *udp_trans = NULL;
    char src_ip[UDP_IPADDR_LEN] = {0};
    UINT16_T src_port = 0;
    char dst_ip[UDP_IPADDR_LEN] = {0};
    UINT16_T dst_port = 0;
    char buffer[1024] = {0};
    int length = 0;
    char flag = 0;
    char opt = 0;
    int opt_index = 0;
    int opt_num = 0;

    while (1)
    {
        if (-1 == (opt = getopt_long(argc, argv, "", longopts, &opt_index)))
        {
            break;
        }

        switch (opt)
        {
            case 0x0010 :
                snprintf(src_ip, sizeof(src_ip), "%s", optarg);
                opt_num++;
                break;
            case 0x0011 :
                src_port = strtol(optarg, NULL, 0);
                opt_num++;
                break;
            case 0x0012 :
                snprintf(dst_ip, sizeof(dst_ip), "%s", optarg);
                opt_num++;
                break;
            case 0x0013 :
                dst_port = strtol(optarg, NULL, 0);
                opt_num++;
                break;
            default :
                printf("Invalid option!\n");
                option_help(argv[0]);
                return -1;
        }
    }

    if (4 != opt_num)
    {
        printf("Option error!\n");
        option_help(argv[0]);
        return -1;
    }

    udp_trans = udp_trans_init(src_ip, src_port, "lo");
    if (!udp_trans)
    {
        printf("failed to init udp trans\n");
        return -1;
    }

    while (1)
    {
        printf("Please input your choose [s<end>/r<ecv>/e<nd>] : \n");

        scanf("%c", &flag);

        if ('s' == flag)
        {
            printf("Please input data you want send : \n");

            scanf("%s", buffer);
        
            length = strlen(buffer);
            
            if (0 > udp_trans_send(udp_trans, dst_ip, dst_port, buffer, length))
            {
                printf("Send error!\n");
                continue;
            }
        }
        else if ('r' == flag)
        {
            if (0 > udp_trans_recv(udp_trans, dst_ip, &dst_port, buffer, sizeof(buffer)))
            {
                printf("Recv error!\n");
                continue;
            }
        
            printf("udp trans recv from : ip<%s> port<%d> data<%s>\n", dst_ip, dst_port, buffer);
        }
        else if ('e' == flag)
        {
            break;
        }
        else if ('\n' == flag)
        {
            continue;
        }
        else
        {
            printf("Invalid choose!\n");
            continue;
        }
    }

    udp_trans_uninit(udp_trans);

    return 0;
}