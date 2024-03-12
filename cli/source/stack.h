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
} sdp_stack_t;

sdp_stack_t *sdp_stack_create(int node_num, int node_size);

int sdp_enstack(sdp_stack_t *stack, void *node, int size);

int sdp_destack(sdp_stack_t *stack, void *node, int size);

int sdp_stack_top(sdp_stack_t *stack, void *node, int size);

void sdp_stack_free(sdp_stack_t *stack);

#endif
