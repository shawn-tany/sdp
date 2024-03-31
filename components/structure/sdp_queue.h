#ifndef SDP_QUEUE
#define SDP_QUEUE

typedef struct 
{
    void *addr;
    int node_size;
    int avail;
    int total;
    int prod;
    int cons;
} SDP_QUEUE_T;

SDP_QUEUE_T *sdp_queue_create(int node_num, int node_size);

void sdp_queue_free(SDP_QUEUE_T *queue);

int sdp_enqueue(SDP_QUEUE_T *queue, void *node, int size);

int sdp_dequeue(SDP_QUEUE_T *queue, void *node, int size);

int sdp_queue_head(SDP_QUEUE_T *queue, void *node, int size);

int sdp_queue_full(SDP_QUEUE_T *queue);

int sdp_queue_empty(SDP_QUEUE_T *queue);

#endif
