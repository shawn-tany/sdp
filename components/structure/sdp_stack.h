#ifndef SDP_STACK
#define SDP_STACK

typedef struct 
{
    void *addr;
    void *top;
    void *prod;
    int offset;
    int avail;
    int total;
} SDP_STACK_T;

SDP_STACK_T *sdp_stack_create(int node_num, int node_size);

int sdp_enstack(SDP_STACK_T *stack, void *node, int size);

int sdp_destack(SDP_STACK_T *stack, void *node, int size);

int SDP_STACK_Top(SDP_STACK_T *stack, void *node, int size);

void sdp_stack_free(SDP_STACK_T *stack);

#endif
