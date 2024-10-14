#ifndef SDP_EVENT
#define SDP_EVENT

#include <pthread.h>

#include "module.h"
#include "pqueue.h"

#define MARK_EVT_TYPE(module, evtid) ((module << 16) | evtid)
#define MARK_EVT_INDEX(evt_type) (evt_type & 0xff)
#define MARK_EVT_MODULE(evt_type) ((evt_type >> 16) & 0xff)

typedef enum 
{
    EVT_BOARD_CPU_TEMP     = MARK_EVT_TYPE(SDP_MD_BOARDINFO, 0),
    EVT_BOARD_CPU_USAGE    = MARK_EVT_TYPE(SDP_MD_BOARDINFO, 1),
    EVT_BOARD_CPU_FREQ     = MARK_EVT_TYPE(SDP_MD_BOARDINFO, 2),
    EVT_BOARD_MEM_USAGE    = MARK_EVT_TYPE(SDP_MD_BOARDINFO, 3),
    EVT_BOARD_DSK_USAGE    = MARK_EVT_TYPE(SDP_MD_BOARDINFO, 4),

    EVT_CMDLINE_EXEC       = MARK_EVT_TYPE(SDP_MD_CMDLINE, 0),
    EVT_CMDLINE_REGISTER   = MARK_EVT_TYPE(SDP_MD_CMDLINE, 1),

    EVT_DIRTRAVER_PUSH     = MARK_EVT_TYPE(SDP_MD_DIRTRAVAL, 0),
    EVT_DIRTRAVER_POP      = MARK_EVT_TYPE(SDP_MD_DIRTRAVAL, 1),

    EVT_THREADPOOL_WORK    = MARK_EVT_TYPE(SDP_MD_THREADPOOL, 0),
    EVT_THREADPOOL_PUSH    = MARK_EVT_TYPE(SDP_MD_THREADPOOL, 1),
    EVT_THREADPOOL_POP     = MARK_EVT_TYPE(SDP_MD_THREADPOOL, 2),

    EVT_TRANS_TCP_INIT     = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 0),
    EVT_TRANS_TCP_SEND     = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 1),
    EVT_TRANS_TCP_RECV     = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 2),
    EVT_TRANS_TCP_UINIT    = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 3),
    EVT_TRANS_UDP_INIT     = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 4),
    EVT_TRANS_UDP_SEND     = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 5),
    EVT_TRANS_UDP_RECV     = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 6),
    EVT_TRANS_UDP_UINIT    = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 7),
    EVT_TRANS_SERIAL_INIT  = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 8),
    EVT_TRANS_SERIAL_SEND  = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 9),
    EVT_TRANS_SERIAL_RECV  = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 10),
    EVT_TRANS_SERIAL_UINIT = MARK_EVT_TYPE(SDP_MD_TRANSMISSION, 11),
} EVENT_TYPE_E;

#define MAX_EVT_INDEX  12
#define EVT_DATA_SIZE 512

typedef int (*event_callback_f)(EVENT_TYPE_E, void *, UINT16_T);

typedef enum 
{
    EVT_CMD_SUBSCRIBE = 0,
    EVT_CMD_UNSUBSCRIBE,
    EVT_CMD_PUBLISH,
} EVT_CMD_E;

typedef struct 
{
    SDP_MODULE_E module;
    EVENT_TYPE_E evt_type;
    EVT_CMD_E    cmd;
    UINT16_T     data_size;
    char         data[0];
} EVT_MSG_S;

typedef struct 
{
    struct 
    {
        int subscribe_list[SDP_MD_NUMBER][MAX_EVT_INDEX];
        PQUEUE_S *pq;
    } module[SDP_MD_NUMBER];

    UINT32_T running;
    pthread_t pid;
    EVT_MSG_S *msg_cache;
} EVT_SERVER_S;

typedef struct 
{
    UINT32_T running;
    pthread_t pid;
    event_callback_f event_handle;
    SDP_MODULE_E module;
    int subscribe_list[MAX_EVT_INDEX];
    PQUEUE_S  *module_pq;
    PQUEUE_S  *event_pq;
    EVT_MSG_S *msg_cache;
} EVT_CLIENT_S;

EVT_SERVER_S *event_server_init(void);

int event_server_work(EVT_SERVER_S *evt_server);

int event_server_uinit(EVT_SERVER_S *evt_server);

EVT_CLIENT_S *event_client_init(SDP_MODULE_E module, event_callback_f evt_handle);

int event_client_subscribe(EVT_CLIENT_S *evt_client, EVENT_TYPE_E evt_list[], UINT16_T evt_num);

int event_client_unsubscribe(EVT_CLIENT_S *evt_client, EVENT_TYPE_E evt_list[], UINT16_T evt_num);

int event_client_publish(EVT_CLIENT_S *evt_client, EVENT_TYPE_E evt_type, void *data, UINT16_T data_size);

int event_client_work(EVT_CLIENT_S *evt_client);

int event_client_uinit(EVT_CLIENT_S *evt_client);

#endif