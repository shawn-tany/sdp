#ifndef DIR_TRAVAL
#define DIR_TRAVAL

#include "sdp_queue.h"
#include "sdp_stack.h"

#define FILE_NAME_SIZE (256)

typedef struct 
{
    char d_name[FILE_NAME_SIZE];
    int is_dir;
    int depth;
} DIR_NODE_T;

typedef struct 
{
    unsigned char  is_dir:1;
    unsigned char  rsv:7;    
    unsigned short last_depth;
    
    SDP_STACK_T *stack;
    SDP_QUEUE_T *queue;
} DIR_CTX_T;

typedef enum
{
    DIR_SUCCESS = 0,
    DIR_ERR_PARAM,
    DIR_ERR_NOSUCH,
    DIR_ERR_MEM,
    DIR_ERR_DIR,
    DIR_ERR_CRTQUEUE,
    DIR_ERR_ENQUEUE,
    DIR_ERR_DEQUEUE,
    DIR_ERR_CRTSTACK,
    DIR_ERR_ENSTACK,
    DIR_ERR_DESTACK,
    DIR_FULL,
    DIR_DEPTH_OVER,
    DIR_ALL_OVER
} DIR_STATUS_T;

DIR_STATUS_T dir_init(DIR_CTX_T * * ctx, const char * path, int file_num);

DIR_STATUS_T dir_uinit(DIR_CTX_T * ctx);

DIR_STATUS_T dir_push(DIR_CTX_T * ctx);

DIR_STATUS_T dir_pop(DIR_CTX_T *ctx, DIR_NODE_T *node);


#endif
