#include <stdio.h>

#include "log.h"

int main(void)
{
    log_init(STREAM_STDOUT, "TEST", LOG_LEVEL_ALL);

    log_print(SDP_MD_LOGGER, LOG_LEVEL_DEBUG, "this is a test demo");

    log_uinit();

    return 0;
}