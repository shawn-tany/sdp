#ifndef SDP_TRIE
#define SDP_TRIE

#define MAX_CHILD_NUM   16
#define MAX_CHILD_DEPTH 16

#define SDP_TRIE_ITEM(a) (sizeof(a)/sizeof(a[0]))

typedef int (*sdp_trie_cmp_func_t)(const void *, int, const void *, int);

typedef int (*sdp_trie_entry_handle_func_t)(const void *, int);

typedef int (*sdp_trie_path_handle_func_t)(const void **, const int *, int);

typedef struct sdp_trie_node
{
    struct sdp_trie_node *node[MAX_CHILD_NUM];
    sdp_trie_entry_handle_func_t func;
    int tail_flag;
    int has_child;
    int depth;
    int node_num;
    int data_len;
    char data[0];
} SDP_TRIE_NODE_T;

typedef struct 
{
    sdp_trie_cmp_func_t cmp_insert;
    sdp_trie_cmp_func_t cmp_found_flex;
    sdp_trie_cmp_func_t cmp_found_fuzz;
} SDP_TRIE_CMP_T;

typedef struct sdp_trie_root
{
    SDP_TRIE_NODE_T root;
    SDP_TRIE_CMP_T  cmp;
    int total_node_num;
} SDP_TRIE_ROOT_T;

int sdp_trie_strcmp(const void *src, int src_size, const void *dst, int dst_size);

SDP_TRIE_ROOT_T *sdp_trie_init(SDP_TRIE_CMP_T cmpset);

int sdp_trie_insert(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num, sdp_trie_entry_handle_func_t func);

SDP_TRIE_NODE_T *sdp_trie_found(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num);

SDP_TRIE_NODE_T *sdp_trie_found_incomplete(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num);

SDP_TRIE_NODE_T *sdp_trie_found_fuzz(SDP_TRIE_ROOT_T *root, void **pattern, int *pattern_len, int ele_num, int fuzz_flag);

int sdp_trie_child_entry(SDP_TRIE_NODE_T *root, sdp_trie_entry_handle_func_t func);

int sdp_trie_path_entry(SDP_TRIE_ROOT_T *root, sdp_trie_entry_handle_func_t func);

int sdp_trie_path_list(SDP_TRIE_ROOT_T *root, sdp_trie_path_handle_func_t func);

int sdp_trie_each_entry_accord_hierarchy(SDP_TRIE_ROOT_T *root, sdp_trie_entry_handle_func_t func);

int sdp_trie_uinit(SDP_TRIE_ROOT_T *root);

#endif
