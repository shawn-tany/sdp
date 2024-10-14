#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#include "common.h"
#include "log.h"

#define LOG_BUFFER_SIZE 512

static struct 
{
    char        *file;
    FILE        *fp;
    char        *title;
    LOG_LEVEL_E  level;

    struct 
    {
        char *sub_title;
    } module[SDP_MD_NUMBER];

} log_config;

static char *log_level_map[] = {
    [LOG_LEVEL_OFF]      = "OFF",
    [LOG_LEVEL_CRITICAL] = "CRITICAL",
    [LOG_LEVEL_ERROR]    = "ERROR",
    [LOG_LEVEL_WARNING]  = "WARNING",
    [LOG_LEVEL_INFO]     = "INFO",
    [LOG_LEVEL_DEBUG]    = "DEBUG",
    [LOG_LEVEL_ALL]      = "ALL",
};

static int stream_open(char *stream)
{
    PTR_CHECK_N1(stream);

    log_config.file = stream;

    if (!strncmp(stream, STREAM_STDIN, strlen(STREAM_STDIN)))
    {
        log_config.fp = stdin;
        return 0;
    }
    else if (!strncmp(stream, STREAM_STDOUT, strlen(STREAM_STDOUT)))
    {
        log_config.fp = stdout;
        return 0;
    }
    else if (!strncmp(stream, STREAM_STDERR, strlen(STREAM_STDERR)))
    {
        log_config.fp = stdin;
        return 0;
    }

    log_config.fp = fopen(stream, "a+");

    if (!log_config.fp)
    {
        return -1;
    }

    return 0;
}

int log_init(char *stream, char *title, LOG_LEVEL_E level)
{
    PTR_CHECK_N1(title);

    int i = 0;

    log_config.title = title;
    log_config.level = level;

    for (i = 0; i < ITEM(log_config.module); ++i)
    {
        log_config.module[i].sub_title = module_name_get(i);
    }

    if (0 > stream_open(stream))
    {
        return -1;
    }

    return 0;
}

int log_level_set(LOG_LEVEL_E level)
{
    if (LOG_LEVEL_ALL < level)
    {
        return -1;
    }

    log_config.level = level;

    return 0;
}

int log_print(SDP_MODULE_E module, LOG_LEVEL_E level, char *fmt, ...)
{
    char buffer[LOG_BUFFER_SIZE];
    va_list va;
    time_t tm;
    struct tm *ltm;
    
    if (log_config.level < level)
    {
        return 0;
    }

    tm  = time(NULL);
    ltm = localtime(&tm);

    va_start(va, fmt);

    vsnprintf(buffer, sizeof(buffer), fmt, va);

    if (!log_config.fp && (0 > stream_open(log_config.file)))
    {
        log_config.fp = stderr;
    }

    fprintf(log_config.fp, "[%04d-%02d-%02d %02d:%02d:%02d], %s, %s, %s, %s\n", 
                            ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, 
                            ltm->tm_hour, ltm->tm_min, ltm->tm_sec, 
                            log_config.title, log_config.module[module].sub_title, 
                            log_level_map[module], buffer);

    va_end(va);
    
    return 0;
}

int log_uinit()
{
    if (log_config.fp)
    {
        fclose(log_config.fp);
    }

    return 0;
}
