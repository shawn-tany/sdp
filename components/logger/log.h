#ifndef SDP_LOG
#define SDP_LOG

#include "module.h"

#define STREAM_STDIN  "stdin"
#define STREAM_STDOUT "stdout"
#define STREAM_STDERR "stderr"

typedef enum 
{
    LOG_LEVEL_OFF = 0,
    LOG_LEVEL_CRITICAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_ALL,
} LOG_LEVEL_E;

int log_init(char *stream, char *title, LOG_LEVEL_E level);

int log_level_set(LOG_LEVEL_E level);

int log_print(SDP_MODULE_E module, LOG_LEVEL_E level, char *fmt, ...);

int log_uinit();

#endif