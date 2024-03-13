#ifndef SDP_QUEUE
#define SDP_QUEUE

typedef struct 
{
    void *addr;
    void *prod;
    void *cons;
    int offset;
    int avail;
    int total;
} sdp_queue_t;

sdp_queue_t *sdp_queue_create(int node_num, int node_size);

void sdp_queue_free(sdp_queue_t *queue);

int sdp_enqueue(sdp_queue_t *queue, void *node, int size);

int sdp_dequeue(sdp_queue_t *queue, void *node, int size);

int sdp_queue_head(sdp_queue_t *queue, void *node, int size);

#endif
