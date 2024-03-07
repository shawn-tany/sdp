#include "tcp_trans.h"
#include "transmission.h"

int tcp_init()
{
    return 0;
}

int tcp_recv()
{
    return 0;
}

int tcp_send()
{
    return 0;
}

int tcp_uninit()
{
    return 0;
}

static MFTP_TRANS_HANDLE_T tcp_handle = {
    .trans_init = tcp_init,
    .trans_recv = tcp_recv,
    .trans_send = tcp_send,
    .trans_uninit = tcp_uninit
};

MFTP_TRANS_HANDLE_REG_CONSTRUCTOR(MFTP_TRANS_TYPE_TCP, tcp_handle);
