#include <stdio.h>
#include <string.h>

#include "tcp_client.h"

int main(int argc, char *argv[])
{
    TCP_CLIENT_T *client = NULL;
    char buffer[1024] = {0};
    int length = 0;

    client = tcp_client_init("127.0.0.1", 8889, "lo");
    if (!client)
    {
        printf("failed to init tcp client\n");
        return -1;
    }

    while (1)
    {
        snprintf(buffer, sizeof(buffer), "tcp client send test aaabbbccc");
        length = strlen(buffer);
        tcp_client_send(client, buffer, length);

        tcp_client_recv(client, buffer, length);
        printf("tcp client recv : %s\n", buffer);
    }

    tcp_client_uninit(client);

    return 0;
}