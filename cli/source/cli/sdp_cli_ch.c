#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "sdp_cli_ch.h"

static int cli_ch_single_register(CLI_CHSET_T *cli_ch_set,
                                  CLI_CH_SINGLE_TYPE_T type, 
                                  CLI_CH_T ch_single)
{
    PTR_CHECK_N1(cli_ch_set);

    cli_ch_set->ch[type] = ch_single;

    return 0;
}

static int cli_ch_multi_input(CLI_CHSET_T *cli_ch_set, char ch)
{
    PTR_CHECK_N1(cli_ch_set);

    if (0x1b != ch)
    {
        return CLI_CH_MULTI_NUM;
    }
    
    ch = getchar();
    if (0x5b != ch)
    {
        return CLI_CH_MULTI_NUM;
    }

    ch = getchar();
    switch (ch)
    {
        case 0x41 :
            return CLI_CH_MULTI_UP;

        case 0x42 :
            return CLI_CH_MULTI_DOWN;

        case 0x43 :
            return CLI_CH_MULTI_RIGHT;

        case 0x44 :
            return CLI_CH_MULTI_LEFT;

        case 0x33 :
            ch = getchar();
            if (0x7e == ch)
            {
                return CLI_CH_MULTI_DELETE;
            }

        case 0x32 :
            ch = getchar();
            if (0x7e == ch)
            {
                return CLI_CH_MULTI_INSERT;
            }
    }

    return CLI_CH_MULTI_NUM;
}

static int cli_ch_single_input(CLI_CHSET_T *cli_ch_set, char ch)
{
    PTR_CHECK_N1(cli_ch_set);

    int i = 0;

    for (i = 0; i < CLI_CH_SINGLE_GENERAL; ++i)
    {            
        if (ch == cli_ch_set->ch[i].ch)
        {
            break;
        }
    }

    return i;
}

CLI_CHSET_T *cli_ch_init(void)
{
    CLI_CHSET_T *cli_ch_set = NULL;

    cli_ch_set = (CLI_CHSET_T *)malloc(sizeof(*cli_ch_set));
    if (!cli_ch_set)
    {
        return NULL;
    }
    memset(cli_ch_set, 0, sizeof(*cli_ch_set));

    CLI_CH_T cli_ch = {0};
    
    /* tab */
    cli_ch.ch = 0x09; 
    cli_ch_single_register(cli_ch_set, CLI_CH_SINGLE_COMPLECTION, cli_ch);

    /* enter */  
    cli_ch.ch = 0x0a;
    cli_ch_single_register(cli_ch_set, CLI_CH_SINGLE_EXECUTE, cli_ch);

    /* ? */ 
    cli_ch.ch = 0x3f;    
    cli_ch_single_register(cli_ch_set, CLI_CH_SINGLE_HELP, cli_ch);

    /* whitespace */
    cli_ch.ch = 0x20; 
    cli_ch_single_register(cli_ch_set, CLI_CH_SINGLE_SEPARATE, cli_ch);

    /* backspace */
    cli_ch.ch = 0x08; 
    cli_ch_single_register(cli_ch_set, CLI_CH_SINGLE_BACKSPACE, cli_ch);

    return cli_ch_set;
}

int cli_ch_uninit(CLI_CHSET_T *cli_ch_set)
{
    PTR_CHECK_N1(cli_ch_set);

    free(cli_ch_set);

    return 0;
}

int cli_ch_input(CLI_CHSET_T *cli_ch_set, char *ch_out)
{
    PTR_CHECK_N1(cli_ch_set);    
    PTR_CHECK_N1(ch_out);

    char ch = 0;
    int ret = 0;

    ch = getchar();
    
    ret = cli_ch_multi_input(cli_ch_set, ch);

    if (0 <= ret)
    {
        switch (ret)
        {
            case CLI_CH_MULTI_UP :
            case CLI_CH_MULTI_DOWN :
            case CLI_CH_MULTI_LEFT :
            case CLI_CH_MULTI_RIGHT :
            case CLI_CH_MULTI_DELETE :
            case CLI_CH_MULTI_INSERT :
                return CLI_CH_MULTI(ret);
        }
    }

    ret = cli_ch_single_input(cli_ch_set, ch);

    if (0 <= ret)
    {
        switch (ret)
        {
            case CLI_CH_SINGLE_COMPLECTION :                
            case CLI_CH_SINGLE_EXECUTE :
            case CLI_CH_SINGLE_HELP :
            case CLI_CH_SINGLE_SEPARATE :
            case CLI_CH_SINGLE_BACKSPACE :
            case CLI_CH_SINGLE_ESCAPE :
                return CLI_CH_SINGLE(ret);
            
            case CLI_CH_SINGLE_GENERAL :
                *ch_out = ch;
                return CLI_CH_SINGLE(ret);

            default :
                return -1;
        }
    }

    return -1;
}
