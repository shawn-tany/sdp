#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "sdp_cli_line.h"

/* base */
int cli_printc(SDP_CLI_LINE_T *cli_line, char ch)
{
    PTR_CHECK_N1(cli_line);

    return write(cli_line->fdout, &ch, 1);
}

int cli_prints(SDP_CLI_LINE_T *cli_line, char *string, int length)
{
    PTR_CHECK_N1(cli_line);
    PTR_CHECK_N1(string);

    if (0 >= length)
    {
        return 0;
    }

    return write(cli_line->fdout, string, length);
}

int cli_cursor_reset(SDP_CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line); 

    cli_line->cursor = 0;

    return 0;
}

/* line option */
int cli_print_line(char *linehead, SDP_CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    if (linehead)
    {
        cli_prints(cli_line, linehead, strlen(linehead));
    }

    if (cli_line->his_line.length)
    {
        cli_prints(cli_line, cli_line->his_line.buff, cli_line->his_line.length);
    }

    return 0;
}

int cli_enter_line(char *linehead, SDP_CLI_LINE_T *cli_line)
{
    PTR_CHECK_N1(cli_line);

    char ch = '\n';

    cli_prints(cli_line, &ch, 1);

    cli_line->his_line.buff[0] = 0;
    cli_line->his_line.length = 0;

    cli_print_line(linehead, cli_line);

    cli_cursor_reset(cli_line);

    return 0;
}

