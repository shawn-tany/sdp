#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BYTE2KB (1024)
#define BYTE2MB (1024 * 1024)
#define BYTE2GB (1024 * 1024 * 1024)

#define TEST_TIMES 100

typedef unsigned long long SIZE_T;

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
    printf("\n    Please input memory size you want to test\n"
           "            example : %s 20GB\n"
           "                      %s 128MB\n"
           "                      %s 1024KB\n", program, program, program);
}

int main(int argc, char *argv[])
{
    int i = 0;
    SIZE_T size = 0;
    SIZE_T block_size = 0;
    SIZE_T last_size = 0;
    void *test_ptr[TEST_TIMES] = {0};
    void *test_last_ptr = NULL;

    if (2 > argc)
    {
        help(argv[0]);
    }

    size = unit_convert_to_byte(argv[1]);
    if (!size)
    {
        printf("ERROR : invalid memory size\n");
        return -1;
    }

    block_size = size / TEST_NUM;

    for (i = 0; i < TEST_NUM; ++i)
    {
        test_ptr[i] = malloc()
    }
}
