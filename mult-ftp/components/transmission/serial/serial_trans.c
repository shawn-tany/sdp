#include "serial_trans.h"
#include "transmission.h"

int serial_init()
{
    return 0;
}

int serial_recv()
{
    return 0;
}

int serial_send()
{
    return 0;
}

int serial_uninit()
{
    return 0;
}

static MFTP_TRANS_HANDLE_T serial_handle = {
    .trans_init = serial_init,
    .trans_recv = serial_recv,
    .trans_send = serial_send,
    .trans_uninit = serial_uninit
};

MFTP_TRANS_HANDLE_REG_CONSTRUCTOR(MFTP_TRANS_TYPE_SERIAL, serial_handle);
