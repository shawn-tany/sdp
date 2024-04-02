#ifndef SDP_CLI_CH
#define SDP_CLI_CH

#define CLI_CH_MULTI_MAX_NUM 6
#define CLI_CH_MULTI_TIMEOUT 1

#define INVALID_GETCHAR_VALUE 0xffffffff

#define CLI_CH_SINGLE(type) (0x100 | type)
#define CLI_CH_MULTI(type) (0x200 | type)

typedef enum 
{
    CLI_CH_SINGLE_COMPLECTION = 0,
    CLI_CH_SINGLE_EXECUTE,
    CLI_CH_SINGLE_HELP,
    CLI_CH_SINGLE_SEPARATE,    
    CLI_CH_SINGLE_BACKSPACE,
    CLI_CH_SINGLE_DELETE,
    CLI_CH_SINGLE_ESCAPE,
    CLI_CH_SINGLE_GENERAL,
} CLI_CH_SINGLE_TYPE_T;

typedef enum 
{
    CLI_CH_MULTI_UP = 0,
    CLI_CH_MULTI_DOWN,
    CLI_CH_MULTI_LEFT,
    CLI_CH_MULTI_RIGHT,
    CLI_CH_MULTI_INSERT,
    CLI_CH_MULTI_DELETE,
    CLI_CH_MULTI_NUM,
} CLI_CH_MULTI_TYPE_T;

typedef struct 
{
    char ch;
} CLI_CH_T;

typedef struct 
{
    CLI_CH_T ch[CLI_CH_SINGLE_GENERAL];
} CLI_CHSET_T;

CLI_CHSET_T *cli_ch_init(void);

int cli_ch_uninit(CLI_CHSET_T *cli_ch_set);

int cli_ch_input(CLI_CHSET_T *cli_ch_set, char *ch_out);

#endif

