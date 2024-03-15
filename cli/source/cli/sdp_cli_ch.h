#ifndef SDP_CLI_CH
#define SDP_CLI_CH

#define SDP_CLI_CH_MULTI_MAX_NUM 6
#define SDP_CLI_CH_MULTI_TIMEOUT 1

#define INVALID_GETCHAR_VALUE 0xffffffff

#define SDP_CLI_CH_SINGLE(type) (0x100 | type)
#define SDP_CLI_CH_MULTI(type) (0x200 | type)

typedef enum 
{
    SDP_CLI_CH_SINGLE_COMPLECTION = 0,
    SDP_CLI_CH_SINGLE_EXECUTE,
    SDP_CLI_CH_SINGLE_HELP,
    SDP_CLI_CH_SINGLE_SEPARATE,    
    SDP_CLI_CH_SINGLE_BACKSPACE,
    SDP_CLI_CH_SINGLE_ESCAPE,
    SDP_CLI_CH_SINGLE_GENERAL,
} SDP_CLI_CH_SINGLE_TYPE_T;

typedef enum 
{
    SDP_CLI_CH_MULTI_UP = 0,
    SDP_CLI_CH_MULTI_DOWN,
    SDP_CLI_CH_MULTI_LEFT,
    SDP_CLI_CH_MULTI_RIGHT,
    SDP_CLI_CH_MULTI_INSERT,
    SDP_CLI_CH_MULTI_DELETE,
    SDP_CLI_CH_MULTI_NUM,
} SDP_CLI_CH_MULTI_TYPE_T;

typedef struct 
{
    char ch;
} SDP_CLI_CH_T;

typedef struct 
{
    SDP_CLI_CH_T ch[SDP_CLI_CH_SINGLE_GENERAL];
} SDP_CLI_CHSET_T;

int sdp_cli_ch_init(SDP_CLI_CHSET_T *cli_ch_set);

int sdp_cli_ch_input(SDP_CLI_CHSET_T *cli_ch_set, char *ch_out);

#endif

