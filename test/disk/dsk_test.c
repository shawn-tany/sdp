#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

#define BYTE2KB (1024)
#define BYTE2MB (1024 * 1024)
#define BYTE2GB (1024 * 1024 * 1024)

#define TEST_TIMES 100
#define TEST_FILENAME_LEN 64
#define TEST_STEP  (usleep(200 * 1000))

#define MIN(a, b) (a > b ? b : a)

#define BLOCK_SIZE (1024 * 1024)

#define DSK_TEST_PATH "/var/tmp/disk_test"

typedef unsigned long long SIZE_T;

static char g_dsk_test_fname[TEST_TIMES][TEST_FILENAME_LEN];
static char g_dsk_test_last_fname[TEST_FILENAME_LEN];
static int running = 1;
static char buffer[BLOCK_SIZE + 1] = {0};

static SIZE_T unit_convert_to_byte(char *data)
{
    int i = 0;
    int length = 0;
    SIZE_T value = 0;

    if (!data)
    {
        return 0;
    }

    length = strlen(data);
    value = strtol(data, NULL, 10);

    for (i = 0; i < length; ++i)
    {
        if (isdigit(data[i]))
        {
            continue;
        }
        else
        {
            break;
        }
    }

    if (i >= length)
    {
        return value;
    }

    if (!strcasecmp(&(data[i]), "GB") || !strcasecmp(&(data[i]), "G"))
    {
        value *= BYTE2GB;
    }
    else if (!strcasecmp(&(data[i]), "MB") || !strcasecmp(&(data[i]), "M"))
    {
        value *= BYTE2MB;
    }
    else if (!strcasecmp(&(data[i]), "KB") || !strcasecmp(&(data[i]), "K"))
    {
        value *= BYTE2KB;
    }

    return value;
}

static void help(char *program)
{
    printf("\n    Please input disk size you want to test\n"
           "            example : %s 20GB\n"
           "                      %s 128MB\n"
           "                      %s 1024KB\n", program, program, program);
}

static int string_auto(char *string, SIZE_T lengh)
{
    int i   = 0;
    int dim = 0;

    for (i = 0; i < lengh; ++i)
    {
        dim = rand() % 0x7E;

        if (dim <= 0x20)
        {
            dim += 0x20;
        }

        string[i] = dim;
    }

    string[i] = 0;

    return 0;
}

int file_create(char *filename, int size)
{
    FILE *fp = NULL;
    int surplus_size = size;
    int write_size = 0;

    if (!filename)
    {
        return -1;
    }

    if (!access(filename, F_OK))
    {
        return -1;
    }

    fp = fopen(filename, "w");
    if (!fp)
    {
        perror("fopen");
        return -1;
    }

    while (surplus_size && running)
    {
        write_size = MIN(surplus_size, BLOCK_SIZE);
    
        string_auto(buffer, write_size);

        fprintf(fp, "%s", buffer);

        surplus_size -= write_size;
    }

    fclose(fp);    

    return 0;
}

int file_remove(char *filename)
{
    if (0 > access(filename, F_OK))
    {
        return -1;
    }

    remove(filename);

    return 0;
}

int disk_test(SIZE_T size)
{
    int i = 0;
    int ret = 0;
    SIZE_T file_size = 0;
    SIZE_T last_size = 0;
    SIZE_T test_times = 0;

    file_size = size / TEST_TIMES;
    last_size  = size % TEST_TIMES;

    if (0 > access(DSK_TEST_PATH, F_OK))
    {
        if (0 > mkdir(DSK_TEST_PATH, 0777))
        {
            return -1;
        }
    }

    while (running)
    {
        for (i = 0; (i < TEST_TIMES) && running; ++i)
        {
            snprintf(g_dsk_test_fname[i], sizeof(g_dsk_test_fname[i]), "%s/tmp_disk_test_%d", DSK_TEST_PATH, i);
            
            if (0 > file_create(g_dsk_test_fname[i], file_size))
            {
                ret = -1;
                break;
            }

            TEST_STEP;
        }

        if (last_size && running && !ret)
        {
            snprintf(g_dsk_test_last_fname, sizeof(g_dsk_test_last_fname), "%s/tmp_disk_test_last", DSK_TEST_PATH);
            
            if (0 > file_create(g_dsk_test_last_fname, last_size))
            {
                ret = -1;
            }

            TEST_STEP;
        }

        if (running)
        {
            printf("disk test %llu times\n", ++test_times);
        }

        for (i = 0; i < TEST_TIMES; ++i)
        {
            if (0 == g_dsk_test_fname[i][0])
            {
                continue;
            }
        
            if (0 > file_remove(g_dsk_test_fname[i]))
            {
                continue;
            }

            g_dsk_test_fname[i][0] = 0;
            
            TEST_STEP;
        }

        if (g_dsk_test_last_fname[0])
        {
            if (0 > file_remove(g_dsk_test_last_fname))
            {
                ret = -1;
            }
        
            TEST_STEP;
        }

        if (0 > ret)
        {
            break;
        }
    }

    return ret;
}

static void test_exit(int arg)
{
    if (SIGINT == arg)
    {
        running = 0;
    }
}

int main(int argc, char *argv[ ])
{
    SIZE_T size = 0;

    if (2 > argc)
    {
        help(argv[0]);
    }

    signal(SIGINT, test_exit);

    size = unit_convert_to_byte(argv[1]);
    if (!size)
    {
        printf("ERROR : invalid memory size\n");
        return -1;
    }

    printf("disk test size %llu bytes\n", size);

    disk_test(size);

    return 0;
}
