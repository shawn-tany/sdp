#ifndef SDP_CLI_LINE
#define SDP_CLI_LINE

#include "sdp_list.h"

#define CLI_MAX_LINE_LEN     512
#define CLI_MAX_LINE_HIS_NUM 36

typedef enum 
{
    CLI_LINE_INPUT_TYPE_ADD = 0,
    CLI_LINE_INPUT_TYPE_REPLACE,
} CLI_LINE_INPUT_TYPE_T;

typedef struct 
{
    CLI_LINE_INPUT_TYPE_T input_type;
} CLI_LINE_STATUS_T;

typedef struct 
{
    sdp_list_t head;
    char buff[CLI_MAX_LINE_LEN];
    int  length;
} SDP_CLI_LINE_CONTENT_T;

typedef struct
{
    int  cursor;
    int  fdout;
    int  replace;
    sdp_list_t list;    
    CLI_LINE_STATUS_T status;
    SDP_CLI_LINE_CONTENT_T  his_line[CLI_MAX_LINE_HIS_NUM];
    SDP_CLI_LINE_CONTENT_T *his_pos;
    SDP_CLI_LINE_CONTENT_T *his_last;
    SDP_CLI_LINE_CONTENT_T  cur_line;
} CLI_LINE_T;

CLI_LINE_T *cli_line_init(int fdout);

int cli_line_uninit(CLI_LINE_T *cli_line);

/* input */
int cli_line_printc(CLI_LINE_T *cli_line, char ch);

int cli_line_prints(CLI_LINE_T *cli_line, char *string, int length);

int cli_line_separate(CLI_LINE_T *cli_line);

int cli_line_backspace(CLI_LINE_T *cli_line);

int cli_line_delete(CLI_LINE_T *cli_line);

int cli_line_insert(char ch, CLI_LINE_T *cli_line);

int cli_line_replace(CLI_LINE_T *cli_line);

/* cursor */
int cli_line_cursor_reset(CLI_LINE_T *cli_line);

int cli_line_cursor_left(CLI_LINE_T *cli_line);

int cli_line_cursor_right(CLI_LINE_T *cli_line);

/* totol line */
int cli_line_reset(CLI_LINE_T *cli_line);

int cli_line_print_line(char *linehead, CLI_LINE_T *cli_line);

int cli_line_new(char *linehead, CLI_LINE_T *cli_line);

int cli_line_his_add(CLI_LINE_T *cli_line);

int cli_line_his_prev(char *linehead, CLI_LINE_T *cli_line);

int cli_line_his_next(char *linehead, CLI_LINE_T *cli_line);

#endif
