#ifndef MULT_FTP_COMMAND
#define MULT_FTP_COMMAND

typedef enum 
{
    MFTP_CMD_LS = 1001,
    MFTP_CMD_CD,
    MFTP_CMD_GET,
    MFTP_CMD_RM,
    MFTP_CMD_TOUCH,
    MFTP_CMD_MKDIR,

} MFTP_COMMAND_T;

#endif