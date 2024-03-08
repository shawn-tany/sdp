#ifndef MLT_QUEUE
#define MLT_QUEUE

typedef struct 
{
    void *addr;
    void *prod;
    void *cons;
    int offset;
    int avail;
    int total;
} mlt_queue_t;

mlt_queue_t *mlt_queue_create(int node_num, int node_size);

void mlt_queue_free(mlt_queue_t *queue);

int mlt_enqueue(mlt_queue_t *queue, void *node, int size);

int mlt_dequeue(mlt_queue_t *queue, void *node, int size);

int mlt_queue_head(mlt_queue_t *queue, void *node, int size);

#endif
