#ifndef SDP_MODULE
#define SDP_MODULE

#define SDP_MODULE_TABLE(out)                           \
    out(SDP_MD_BOARDINFO,     0,    "BOARDINFO"     )   \
    out(SDP_MD_CMDLINE,       1,    "CMDLINE"       )   \
    out(SDP_MD_DIRTRAVAL,     2,    "DIRTRAVAL"     )   \
    out(SDP_MD_EVENT,         3,    "EVENT"         )   \
    out(SDP_MD_LOGGER,        4,    "LOGGER"        )   \
    out(SDP_MD_THREADPOOL,    5,    "THREADPOOL"    )   \
    out(SDP_MD_TRANSMISSION,  6,    "TRANSMISSION"  )   \
    out(SDP_MD_NUMBER,        7,    ""              )   \

#define out_micro_module(m, v, i) m = v,

typedef enum { SDP_MODULE_TABLE(out_micro_module) } SDP_MODULE_E;

char *module_name_get(SDP_MODULE_E module);

#endif