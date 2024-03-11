#include "transmission.h"
#include "serial_trans.h"
#include "udp_trans.h"
#include "tcp_trans.h"

MFTP_TRANS_HANDLE_T trans_handle_list[MFTP_TRANS_TYPE_NUM];

void trans_handle_register(MFTP_TRANS_TYPE_T trans_type, MFTP_TRANS_HANDLE_T trans_handle)
{
    if ((UINT32_T)trans_type < MFTP_TRANS_TYPE_NUM)
    {
        trans_handle_list[trans_type] = trans_handle;
    }
}

int trans_init(MFTP_TRANS_DESC_T *trans_desc)
{
    PTR_CHECK_N1(trans_desc);

    MFTP_TRANS_TYPE_T trans_type = trans_desc->trans_type;
    int ret = 0;

    if ((UINT32_T)trans_type < MFTP_TRANS_TYPE_NUM)
    {
        return -1;
    }

    ret = trans_handle_list[trans_desc->trans_type].trans_init(&trans_desc->desc);

    return ret;
}

int trans_recv(MFTP_TRANS_DESC_T *trans_desc, MFTP_MSG_TRANS_T *msg)
{    
    PTR_CHECK_N1(trans_desc);    
    PTR_CHECK_N1(msg);

    MFTP_TRANS_TYPE_T trans_type = trans_desc->trans_type;
    int ret = 0;

    if ((UINT32_T)trans_type < MFTP_TRANS_TYPE_NUM)
    {
        return -1;
    }

    ret = trans_handle_list[trans_type].trans_recv(&trans_desc->desc, msg);

    return ret;
}

int trans_send(MFTP_TRANS_DESC_T *trans_desc, MFTP_MSG_TRANS_T *msg)
{
    PTR_CHECK_N1(trans_desc);    
    PTR_CHECK_N1(msg);

    MFTP_TRANS_TYPE_T trans_type = trans_desc->trans_type;
    int ret = 0;

    if ((UINT32_T)trans_type < MFTP_TRANS_TYPE_NUM)
    {
        return -1;
    }

    ret = trans_handle_list[trans_type].trans_send(&trans_desc->desc, msg);

    return ret;
}

int trans_uninit(MFTP_TRANS_DESC_T *trans_desc)
{
    PTR_CHECK_N1(trans_desc);    

    MFTP_TRANS_TYPE_T trans_type = trans_desc->trans_type;
    int ret = 0;

    if ((UINT32_T)trans_type < MFTP_TRANS_TYPE_NUM)
    {
        return -1;
    }

    ret = trans_handle_list[trans_type].trans_uninit(&trans_desc->desc);

    return ret;
}
