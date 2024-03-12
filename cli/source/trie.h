#ifndef TRIE
#define TRIE

#define MAX_CHILD_NUM   16
#define MAX_CHILD_DEPTH 16

#define TRIE_ITEM(a) (sizeof(a)/sizeof(a[0]))

typedef int (*trie_cmp_func_t)(const void *, int, const void *, int);

typedef int (*trie_entry_handle_func_t)(const void *, int);

typedef int (*trie_path_handle_func_t)(const void **, const int *, int);

typedef struct trie_node
{
    struct trie_node *node[MAX_CHILD_NUM];
    trie_entry_handle_func_t func;
    int tail_flag;
    int depth;
    int node_num;
    int data_len;
    char data[0];
} TRIE_NODE_T;

typedef struct trie_root
{
    TRIE_NODE_T root;
    trie_cmp_func_t cmp;
    int total_node_num;
} TRIE_ROOT_T;

int trie_strcmp(const void *src, int src_size, const void *dst, int dst_size);

TRIE_ROOT_T *trie_init(trie_cmp_func_t cmp);

int trie_insert(TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num, trie_entry_handle_func_t func);

TRIE_NODE_T *tire_found(TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num);

int trie_child_entry(TRIE_NODE_T *root, trie_entry_handle_func_t func);

int trie_path_entry(TRIE_ROOT_T *root, trie_entry_handle_func_t func);

int trie_path_list(TRIE_ROOT_T *root, trie_path_handle_func_t func);

int trie_each_entry_accord_hierarchy(TRIE_ROOT_T *root, trie_entry_handle_func_t func);

int trie_uinit(TRIE_ROOT_T *root);

#endif