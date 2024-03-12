
#include "stdio.h"
#include "string.h"

#include "trie.h"

static struct 
{
    char *pattern[32];
    int   pattern_num;
} pattern_list[] = 
{
    {{"hello", "world"}, 2},
    {{"this", "is", "my", "trie"}, 4},
    {{"this", "is", "my"}, 3},
    {{"hello", "worldd"}, 2},
    {{"hello", "worldd", "hhh"}, 3},
    {{"hello", "worldd", "ggg"}, 3},
    {{"hello", "worldd", "ggg", "hhh"}, 4},
    {{"this", "is"}, 2},
    {{"this", "is", "my", "test"}, 4},
    {{"i", "want", "so", "much", "money"}, 5},
    {{"wu", "qi", "biao", "da", "shuai", "bi"}, 6},
    {{"niu", "le", "xiao", "diao", "mao"}, 5}
};

static struct 
{
    char *pattern[32];
    int   pattern_num;
} invalid_pattern_list[] = 
{
    {{"hello", "firend"}, 2},
    {{"this", "is", "your", "trie"}, 4},
    {{"this", "is", "your"}, 3},
    {{"hello", "firendd"}, 2},
    {{"hello", "firendd", "hhh"}, 3},
    {{"hello", "firendd", "ggg"}, 3},
    {{"hello", "firendd", "ggg", "hhh"}, 4},
    {{"that", "is"}, 2},
    {{"that", "is", "my", "test"}, 4},
    {{"i", "need", "so", "much", "money"}, 5},
    {{"wu", "qi", "da", "shuai", "bi"}, 4},
    {{"niu", "le", "diao", "mao"}, 4}
};

int trie_entry_print(const void *data, int data_len)
{
    printf("%s\n", (char *)data);

    return 0;
}

int trie_path_print(const void **data_array, const int *data_len_array, const int item_num)
{
    int i = 0;

    for (i = 0; i < item_num; ++i)
    {
        printf("%s ", (const char *)data_array[i]);
    }

    printf("\n");

    return 0;
}

int main()
{
    int i = 0;
    int j = 0;
    TRIE_ROOT_T *root = NULL;
    TRIE_NODE_T *node = NULL;

    int pattern_len[32] = {0};

    root = trie_init(trie_strcmp);
    if (!root)
    {
        printf("trie init failed\n");
        return -1;
    }

    for (i = 0; i < TRIE_ITEM(pattern_list); ++i)
    {
        for (j = 0; j < pattern_list[i].pattern_num; ++j)
        {
            pattern_len[j] = strlen(pattern_list[i].pattern[j]) + 1;
        }
        trie_insert(root, (void **)pattern_list[i].pattern, pattern_len, pattern_list[i].pattern_num, NULL);
    }

    for (i = 0; i < TRIE_ITEM(pattern_list); ++i)
    {
        for (j = 0; j < pattern_list[i].pattern_num; ++j)
        {
            pattern_len[j] = strlen(pattern_list[i].pattern[j]) + 1;
        }
        if ((node = tire_found(root, (void **)pattern_list[i].pattern, pattern_len, pattern_list[i].pattern_num)))
        {
            printf("search pattern%d\n", i + 1);
        }
    }

    for (i = 0; i < TRIE_ITEM(invalid_pattern_list); ++i)
    {
        for (j = 0; j < invalid_pattern_list[i].pattern_num; ++j)
        {
            pattern_len[j] = strlen(invalid_pattern_list[i].pattern[j]) + 1;
        }
        if ((node = tire_found(root, (void **)invalid_pattern_list[i].pattern, pattern_len, invalid_pattern_list[i].pattern_num)))
        {
            printf("search invalid pattern%d\n", i + 1);
        }
    }

    trie_path_entry(root, trie_entry_print);

    printf("-------------------------------\n");

    trie_path_list(root, trie_path_print);

    printf("-------------------------------\n");

    trie_each_entry_accord_hierarchy(root, trie_entry_print);

    trie_uinit(root);

    return 0;
}