#ifndef MFTP_TRANSMISSION
#define MFTP_TRANSMISSION

#include "common.h"
#include "msg.h"
#include "tcp_trans.h"

typedef enum 
{
    MFTP_TRANS_TYPE_SERIAL,
    MFTP_TRANS_TYPE_UDP,
    MFTP_TRANS_TYPE_TCP,
    MFTP_TRANS_TYPE_NUM,
} MFTP_TRANS_TYPE_T;

typedef enum 
{
    MFTP_ROLE_SERVER = 0,
    MFTP_ROLE_CLIENT,
} MFTP_ROLE_T;

typedef enum 
{
    MFTP_DIRECTION_RECV = 0,
    MFTP_DIRECTION_SEND,
} MFTP_DIRECTION_T;

typedef struct 
{
    MFTP_ROLE_T role;
    MFTP_TRANS_TYPE_T trans_type;

    union
    {
        MFTP_TCP_DESC_T tcp;
    } desc;
    
} MFTP_TRANS_DESC_T;

typedef int (*trans_init_func)(void *);
typedef int (*trans_recv_func)(void *, void *);
typedef int (*trans_send_func)(void *, void *);
typedef int (*trans_uninit_func)(void *);

typedef struct 
{
    trans_init_func trans_init;
    trans_recv_func trans_recv;
    trans_send_func trans_send;
    trans_uninit_func trans_uninit;
} MFTP_TRANS_HANDLE_T;

void trans_handle_register(MFTP_TRANS_TYPE_T trans_type, MFTP_TRANS_HANDLE_T trans_handle);

int trans_init(MFTP_TRANS_DESC_T *trans_desc);

int trans_recv(MFTP_TRANS_DESC_T *trans_desc, MFTP_MSG_T *msg);

int trans_send(MFTP_TRANS_DESC_T *trans_desc, MFTP_MSG_T *msg);

int trans_uninit(MFTP_TRANS_DESC_T *trans_desc);


#define MFTP_TRANS_HANDLE_REG_CONSTRUCTOR(trans_type, trans_handle)     \
    static void __attribute__((constructor)) MFTP_TRANS_HANDLE_REG() {  \
        trans_handle_register(trans_type, trans_handle);                \
    }

#endif
