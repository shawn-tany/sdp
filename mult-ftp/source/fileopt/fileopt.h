#ifndef MFTP_FILE_OPT
#define MFTP_FILE_OPT

#define MAX_FILENAME_SIZE 512

typedef enum 
{
    MFTP_CMD_NONE = -1,
    MFTP_CMD_LS = 0,
    MFTP_CMD_CD,
    MFTP_CMD_DOWNLOAD,
    MFTP_CMD_UPDATA,
    MFTP_CMD_RM,
    MFTP_CMD_TOUCH,
    MFTP_CMD_MKDIR,
    MFTP_CMD_NUM,

} MFTP_COMMAND_T;

typedef enum 
{
    MFTP_ROLE_SERVER = 0,
    MFTP_ROLE_CLIENT,
} MFTP_ROLE_T;

typedef struct 
{
    MFTP_COMMAND_T cmd;
    char src[MAX_FILENAME_SIZE];
    char dst[MAX_FILENAME_SIZE];
} MFTP_FILE_OPT_T;

typedef int (*file_handle_func)(char *);

typedef struct 
{
    file_handle_func func;
} MFTP_FILE_HANDLE_T;

int mftp_server_init();

int mftp_server_uninit();

int mftp_server_loop();

int mftp_client_init();

int mftp_client_uninit();

int mftp_client_loop();

#endif
