#ifndef MFTP_MSG
#define MFTP_MSG

#include "common.h"

typedef enum 
{
    /* msg */
    MFTP_MSG_TYPE_MSG_INFO = 0,

    /* file */
    MFTP_MSG_TYPE_FILE_INFO,
    MFTP_MSG_TYPE_FILE_DATA,
    MFTP_MSG_TYPE_END
} MFTP_MSG_TYPE_T;

typedef struct 
{
    UINT64_T totol_data_size;
} MFTP_MSG_INFO_T;

typedef struct 
{
    MFTP_MSG_TYPE_T msg_type;
} MFTP_MSG_HEADER_T;

typedef struct 
{
    MFTP_MSG_INFO_T *msg_info;
    MFTP_MSG_HEADER_T *msg_header;

    UINT64_T data_size;
    UINT64_T data_len;
    char data[0];
} MFTP_MSG_T;

typedef void*(*mftp_msg_handle_func)(void *);

#endif
