#include "udp_trans.h"
#include "transmission.h"

int udp_init()
{
    return 0;
}

int udp_recv()
{
    return 0;
}

int udp_send()
{
    return 0;
}

int udp_uninit()
{
    return 0;
}

static MFTP_TRANS_HANDLE_T udp_handle = {
    .trans_init = udp_init,
    .trans_recv = udp_recv,
    .trans_send = udp_send,
    .trans_uninit = udp_uninit
};

MFTP_TRANS_HANDLE_REG_CONSTRUCTOR(MFTP_TRANS_TYPE_UDP, udp_handle);
