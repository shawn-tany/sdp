#include "fileopt.h"
#include "transmission.h"

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

int mftp_server_init()
{
    trans_init();
}

int mftp_server_uninit()
{
    trans_uninit();
}

int mftp_server_loop()
{
    while (1)
    {
        MFTP_FILE_OPT_T *file_opt;

        trans_recv();

        mftp_file_opt_handle(file_opt);

        trans_send();
    }

    return 0;
}

int mftp_client_init()
{
    trans_init();
}

int mftp_client_uninit()
{
    trans_uninit();
}

int mftp_client_loop()
{
    while (1)
    {
        MFTP_FILE_OPT_T *file_opt;

        trans_send();

        mftp_file_opt_handle(file_opt);

        trans_recv();
    }

    return 0;
}
