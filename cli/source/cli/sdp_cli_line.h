#ifndef SDP_CLI_LINE
#define SDP_CLI_LINE

#define SDP_CLI_MAX_LINE_LEN  512

typedef struct 
{
    char buff[SDP_CLI_MAX_LINE_LEN];
    int  length;
} SDP_CLI_HISLINE_T;

typedef struct
{
    int  cursor;
    int  fdout;
    SDP_CLI_HISLINE_T his_line;
} SDP_CLI_LINE_T;

int cli_printc(SDP_CLI_LINE_T *cli_line, char ch);

int cli_prints(SDP_CLI_LINE_T *cli_line, char *string, int length);

int cli_cursor_reset(SDP_CLI_LINE_T *cli_line);

int cli_print_line(char *linehead, SDP_CLI_LINE_T *cli_line);

int cli_enter_line(char *linehead, SDP_CLI_LINE_T *cli_line);

#endif
