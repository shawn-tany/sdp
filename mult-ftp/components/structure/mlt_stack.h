#ifndef MLT_STACK
#define MLT_STACK

typedef struct 
{
    void *addr;
    void *top;
    void *prod;
    int offset;
    int avail;
    int total;
} mlt_stack_t;

mlt_stack_t *mlt_stack_create(int node_num, int node_size);

int mlt_enstack(mlt_stack_t *stack, void *node, int size);

int mlt_destack(mlt_stack_t *stack, void *node, int size);

int mlt_stack_top(mlt_stack_t *stack, void *node, int size);

void mlt_stack_free(mlt_stack_t *stack);

#endif
