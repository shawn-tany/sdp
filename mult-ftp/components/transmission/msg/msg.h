#ifndef MFTP_MSG
#define MFTP_MSG

#include "common.h"

typedef enum 
{
    MFTP_MSG_TYPE_HEAD = 0,
    MFTP_MSG_TYPE_FILE_INFO,
    MFTP_MSG_TYPE_FILE_DATA,
    MFTP_MSG_TYPE_END
} MFTP_MSG_TYPE_T;

typedef struct 
{
    UINT64_T totol_data_size;
} MFTP_MSG_HEADER_T;

typedef struct 
{
    MFTP_MSG_TYPE_T msg_type;
    UINT64_T data_size;
    UINT64_T data_len;
    char data[0];
} MFTP_MSG_TRANS_T;

#endif
