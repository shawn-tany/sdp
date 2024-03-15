#include <stdio.h>
#include <unistd.h>

#include "common.h"
#include "sdp_cli_ch.h"

static int sdp_cli_ch_single_register(SDP_CLI_CHSET_T *cli_ch_set,
                                             SDP_CLI_CH_SINGLE_TYPE_T type, 
                                             SDP_CLI_CH_T ch_single)
{
    PTR_CHECK_N1(cli_ch_set);

    cli_ch_set->ch[type] = ch_single;

    return 0;
}

static int sdp_cli_ch_multi_input(SDP_CLI_CHSET_T *cli_ch_set, char ch)
{
    PTR_CHECK_N1(cli_ch_set);

    if (0x1b != ch)
    {
        return SDP_CLI_CH_MULTI_NUM;
    }
    
    ch = getchar();
    if (0x5b != ch)
    {
        return SDP_CLI_CH_MULTI_NUM;
    }

    ch = getchar();
    switch (ch)
    {
        case 0x41 :
            return SDP_CLI_CH_MULTI_UP;

        case 0x42 :
            return SDP_CLI_CH_MULTI_DOWN;

        case 0x43 :
            return SDP_CLI_CH_MULTI_RIGHT;

        case 0x44 :
            return SDP_CLI_CH_MULTI_LEFT;

        case 0x33 :
            ch = getchar();
            if (0x7e == ch)
            {
                return SDP_CLI_CH_MULTI_DELETE;
            }

        case 0x32 :
            ch = getchar();
            if (0x7e == ch)
            {
                return SDP_CLI_CH_MULTI_INSERT;
            }
    }

    return SDP_CLI_CH_MULTI_NUM;
}

static int sdp_cli_ch_single_input(SDP_CLI_CHSET_T *cli_ch_set, char ch)
{
    PTR_CHECK_N1(cli_ch_set);

    int i = 0;

    for (i = 0; i < SDP_CLI_CH_SINGLE_GENERAL; ++i)
    {            
        if (ch == cli_ch_set->ch[i].ch)
        {
            break;
        }
    }

    return i;
}

int sdp_cli_ch_init(SDP_CLI_CHSET_T *cli_ch_set)
{
    PTR_CHECK_N1(cli_ch_set);

    SDP_CLI_CH_T cli_ch = {0};
    
    /* tab */
    cli_ch.ch = 0x09; 
    sdp_cli_ch_single_register(cli_ch_set, SDP_CLI_CH_SINGLE_COMPLECTION, cli_ch);

    /* enter */  
    cli_ch.ch = 0x0a;
    sdp_cli_ch_single_register(cli_ch_set, SDP_CLI_CH_SINGLE_EXECUTE, cli_ch);

    /* ? */ 
    cli_ch.ch = 0x3f;    
    sdp_cli_ch_single_register(cli_ch_set, SDP_CLI_CH_SINGLE_HELP, cli_ch);

    /* whitespace */
    cli_ch.ch = 0x20; 
    sdp_cli_ch_single_register(cli_ch_set, SDP_CLI_CH_SINGLE_SEPARATE, cli_ch);

    /* backspace */
    cli_ch.ch = 0x08; 
    sdp_cli_ch_single_register(cli_ch_set, SDP_CLI_CH_SINGLE_BACKSPACE, cli_ch);

    return 0;
}

int sdp_cli_ch_input(SDP_CLI_CHSET_T *cli_ch_set, char *ch_out)
{
    PTR_CHECK_N1(cli_ch_set);    
    PTR_CHECK_N1(ch_out);

    char ch = 0;
    int ret = 0;

    ch = getchar();
    
    ret = sdp_cli_ch_multi_input(cli_ch_set, ch);

    if (0 <= ret)
    {
        switch (ret)
        {
            case SDP_CLI_CH_MULTI_UP :
            case SDP_CLI_CH_MULTI_DOWN :
            case SDP_CLI_CH_MULTI_LEFT :
            case SDP_CLI_CH_MULTI_RIGHT :
            case SDP_CLI_CH_MULTI_DELETE :
            case SDP_CLI_CH_MULTI_INSERT :
                return SDP_CLI_CH_MULTI(ret);
        }
    }

    ret = sdp_cli_ch_single_input(cli_ch_set, ch);

    if (0 <= ret)
    {
        switch (ret)
        {
            case SDP_CLI_CH_SINGLE_COMPLECTION :                
            case SDP_CLI_CH_SINGLE_EXECUTE :
            case SDP_CLI_CH_SINGLE_HELP :
            case SDP_CLI_CH_SINGLE_SEPARATE :
            case SDP_CLI_CH_SINGLE_BACKSPACE :
            case SDP_CLI_CH_SINGLE_ESCAPE :
                return SDP_CLI_CH_SINGLE(ret);
            
            case SDP_CLI_CH_SINGLE_GENERAL :
                *ch_out = ch;
                return SDP_CLI_CH_SINGLE(ret);

            default :
                return -1;
        }
    }

    return -1;
}
