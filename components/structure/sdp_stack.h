#ifndef SDP_STACK
#define SDP_STACK

typedef struct 
{
    void *addr;
    int node_size;
    int avail;
    int total;
    int top;
    int prod;
} SDP_STACK_T;

SDP_STACK_T *sdp_stack_create(int node_num, int node_size);

int sdp_enstack(SDP_STACK_T *stack, void *node, int size);

int sdp_destack(SDP_STACK_T *stack, void *node, int size);

int sdp_stack_top(SDP_STACK_T *stack, void *node, int size);

void sdp_stack_free(SDP_STACK_T *stack);

#endif
