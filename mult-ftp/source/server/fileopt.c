#include <stdio.h>

#include "fileopt.h"

/* file handle */
static int mftp_file_handle_ll(char *path)
{
    return 0;
}

static int mftp_file_handle_cd(char *path)
{
    return 0;
}

static int mftp_file_handle_download(char *path)
{
    return 0;
}

static int mftp_file_handle_updata(char *path)
{
    return 0;
}

static int mftp_file_handle_rm(char *path)
{
    return 0;
}

static int mftp_file_handle_touch(char *path)
{
    return 0;
}

static int mftp_file_handle_mkdir(char *path)
{
    return 0;
}

static MFTP_FILE_HANDLE_T file_handle_list[] = 
{
    [MFTP_CMD_LS] = { mftp_file_handle_ll },
    [MFTP_CMD_CD] = { mftp_file_handle_cd },
    [MFTP_CMD_DOWNLOAD] = { mftp_file_handle_download },
    [MFTP_CMD_UPDATA] = { mftp_file_handle_updata },
    [MFTP_CMD_RM] = { mftp_file_handle_rm },
    [MFTP_CMD_TOUCH] = { mftp_file_handle_touch },
    [MFTP_CMD_MKDIR] = { mftp_file_handle_mkdir }
};

static int mftp_file_opt_handle(MFTP_FILE_OPT_T *file_opt)
{
    PTR_CHECK_N1(file_opt);

    if (file_opt->cmd < MFTP_CMD_NONE ||
        file_opt->cmd > MFTP_CMD_NUM)
    {
        return -1;
    }

    if (0 > file_handle_list[file_opt->cmd].func(file_opt->src))
    {
        return -1;
    }

    return 0;
}

static int mftp_server_init()
{
    MFTP_TRANS_DESC_T *trans_desc = NULL;

    trans_init(trans_desc);

    return 0;
}

static int mftp_server_uninit()
{
    MFTP_TRANS_DESC_T *trans_desc = NULL;

    trans_uninit(trans_desc);

    return 0;
}

static int mftp_server_loop()
{
    MFTP_TRANS_DESC_T *trans_desc = NULL;
    MFTP_MSG_T *msg = NULL;

    while (1)
    {
        MFTP_FILE_OPT_T *file_opt = NULL;

        trans_recv(trans_desc, msg);

        mftp_file_opt_handle(file_opt);

        trans_send(trans_desc, msg);
    }

    return 0;
}

int mftp_work()
{
    mftp_server_init();

    mftp_server_loop();

    mftp_server_uninit();

    return 0;
}
