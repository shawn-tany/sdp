#include <stdio.h>
#include <string.h>

#include "tcp_server.h"

int recv_complete_callback_test(void *data, int data_len)
{
    PTR_CHECK_N1(data);

    printf("recv data : %s\n", (char *)data);

    return 0;
}

int send_encap_callback_test(void *data, int data_size, int *data_len)
{
    PTR_CHECK_N1(data);
    PTR_CHECK_N1(data_len);

    char *str = "aaaabbbbccccddddd";

    snprintf((char *)data, data_size, "%s", str);
    *data_len = strlen(str);

    return 0;
}

int main(int argc, char *argv[])
{
    TCP_SERVER_T *server = NULL;
    
    TCP_SERVER_FUNC_T func = {
        .recv_complete_callback = recv_complete_callback_test,
        .send_encap_callback = send_encap_callback_test,
    };

    server = tcp_server_init("127.0.0.1", 8889, "lo");
    if (!server)
    {
        printf("failed to init tcp server\n");
        return -1;
    }

    tcp_server_loop(server, &func);

    return 0;
}