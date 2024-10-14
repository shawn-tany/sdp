#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "event.h"

static int event_server_subscribe_handle(EVT_SERVER_S *evt_server)
{
    int i = 0;
    UINT16_T sub_module = 0;
    UINT16_T sub_evtidx = 0;
    EVENT_TYPE_E evt_type = 0;

    EVT_MSG_S *msg = evt_server->msg_cache;

    for (i = 0; i < (msg->data_size / sizeof(EVENT_TYPE_E)); ++i)
    {
        evt_type = ((EVENT_TYPE_E *)msg->data)[i];

        sub_module = MARK_EVT_MODULE(evt_type);
        sub_evtidx = MARK_EVT_INDEX(evt_type);

        if (SDP_MD_NUMBER <= sub_module || MAX_EVT_INDEX <= sub_evtidx)
        {
            continue;
        }

        printf("subscribe module %s event 0x%x\n", module_name_get(sub_module), evt_type);

        evt_server->module[msg->module].subscribe_list[sub_module][sub_evtidx] = TRUE;
    }

    return 0;
}

static int event_server_unsubscribe_handle(EVT_SERVER_S *evt_server)
{
    int i = 0;
    UINT16_T sub_module = 0;
    UINT16_T sub_evtidx = 0;
    EVENT_TYPE_E evt_type = 0;

    EVT_MSG_S *msg = evt_server->msg_cache;

    for (i = 0; i < (msg->data_size / sizeof(EVENT_TYPE_E)); ++i)
    {
        evt_type = ((EVENT_TYPE_E *)msg->data)[i];

        sub_module = MARK_EVT_MODULE(evt_type);
        sub_evtidx = MARK_EVT_INDEX(evt_type);

        if (SDP_MD_NUMBER <= sub_module || MAX_EVT_INDEX <= sub_evtidx)
        {
            continue;
        }

        printf("unsubscribe module %s event 0x%x\n", module_name_get(sub_module), evt_type);

        evt_server->module[msg->module].subscribe_list[sub_module][sub_evtidx] = FALSE;
    }

    return 0;
}

static int event_server_publish_handle(EVT_SERVER_S *evt_server)
{
    int i = 0;
    EVT_MSG_S *msg = evt_server->msg_cache;
    UINT16_T sub_module = MARK_EVT_MODULE(msg->evt_type);
    UINT16_T sub_evtidx = MARK_EVT_INDEX(msg->evt_type);

    if (SDP_MD_NUMBER <= sub_module || MAX_EVT_INDEX <= sub_evtidx)
    {
        return -1;
    }

    for (i = 0; i < SDP_MD_NUMBER; ++i)
    {
        if (evt_server->module[i].subscribe_list[sub_module][sub_evtidx])
        {
            printf("publish event 0x%x to module %s\n", msg->evt_type, module_name_get(i));

            pqueue_produce(evt_server->module[i].pq, msg, sizeof(EVT_MSG_S) + EVT_DATA_SIZE);
        }
    }

    return 0;
}

static int event_msg_handle(EVT_SERVER_S *evt_server)
{
    switch (evt_server->msg_cache->cmd)
    {
        case EVT_CMD_SUBSCRIBE :
            event_server_subscribe_handle(evt_server);
            break;

        case EVT_CMD_UNSUBSCRIBE :
            event_server_unsubscribe_handle(evt_server);
            break;

        case EVT_CMD_PUBLISH :
            event_server_publish_handle(evt_server);
            break;

        default :
            break;
    }

    return 0;
}

static void *event_server_loop(void *arg)
{
    EVT_SERVER_S *evt_server = arg;
    PQUEUE_S *pq = evt_server->module[SDP_MD_EVENT].pq;

    while (evt_server->running)
    {
        if (!pqueue_consume_timeout(pq, evt_server->msg_cache, sizeof(EVT_MSG_S) + EVT_DATA_SIZE, 5))
        {
            event_msg_handle(evt_server);
        }
    }

    return NULL;
}

EVT_SERVER_S *event_server_init(void)
{
    int i   = 0;
    int ret = 0;
    EVT_SERVER_S *evt_server = NULL;

    evt_server = (EVT_SERVER_S *)malloc(sizeof(*evt_server));

    if (!evt_server)
    {
        printf("failed to alloc event server\n");
        return NULL;
    }

    memset(evt_server, 0, sizeof(*evt_server));

    for (i = 0; i < SDP_MD_NUMBER; ++i)
    {
        evt_server->module[i].pq = pqueue_create(module_name_get(i), sizeof(EVT_MSG_S) + EVT_DATA_SIZE, 100);

        if (!evt_server->module[i].pq)
        {
            printf("failed to create %s pqueue\n", module_name_get(i));
            ret = -1;
            break;
        }
    }

    evt_server->msg_cache = (EVT_MSG_S *)malloc(sizeof(EVT_MSG_S) + EVT_DATA_SIZE);

    if (!evt_server->msg_cache)
    {
        printf("failed to alloc event server msg cache\n");
        ret = -1;
    }

    if (-1 == ret)
    {
        event_server_uinit(evt_server);
        return NULL;
    }

    evt_server->running = TRUE;

    return evt_server;
}

int event_server_work(EVT_SERVER_S *evt_server)
{
    int ret = 0;

    ret = pthread_create(&evt_server->pid, NULL, event_server_loop, evt_server);

    if (0 > ret)
    {
        printf("failed to create event thread loop\n");
        return -1;
    }

    return 0;
}

int event_server_uinit(EVT_SERVER_S *evt_server)
{
    int i = 0;

    if (evt_server)
    {
        evt_server->running = FALSE;

        pthread_join(evt_server->pid, NULL);

        for (i = 0; i < SDP_MD_NUMBER; ++i)
        {
            pqueue_destory(evt_server->module[i].pq);
        }

        if (evt_server->msg_cache)
        {
            free(evt_server->msg_cache);
        }

        free(evt_server);
    }

    return 0;
}

static void *event_client_loop(void *arg)
{
    EVT_CLIENT_S *evt_client = (EVT_CLIENT_S *)arg;
    EVT_MSG_S    *msg        = NULL;

    while (evt_client->running)
    {
        if (!pqueue_consume_timeout(evt_client->module_pq, evt_client->msg_cache, sizeof(EVT_MSG_S) + EVT_DATA_SIZE, 5))
        {
            msg = evt_client->msg_cache;

            evt_client->event_handle(msg->evt_type, msg->data, msg->data_size);
        }
    }

    return NULL;
}

EVT_CLIENT_S *event_client_init(SDP_MODULE_E module, event_callback_f evt_handle)
{
    EVT_CLIENT_S *evt_client = NULL;

    evt_client = (EVT_CLIENT_S *)malloc(sizeof(*evt_client));

    if (!evt_client)
    {
        printf("failed to alloc event client\n");
        return NULL;
    }

    memset(evt_client, 0, sizeof(*evt_client));

    evt_client->event_handle = evt_handle;
    evt_client->module       = module;

    evt_client->module_pq = pqueue_create(module_name_get(module), sizeof(EVT_MSG_S) + EVT_DATA_SIZE, 100);

    if (!evt_client->module_pq)
    {
        printf("failed to create %s pqueue\n", module_name_get(module));
        event_client_uinit(evt_client);
        return NULL;
    }

    evt_client->event_pq = pqueue_create(module_name_get(SDP_MD_EVENT), sizeof(EVT_MSG_S) + EVT_DATA_SIZE, 100);

    if (!evt_client->event_pq)
    {
        printf("failed to create %s pqueue\n", module_name_get(SDP_MD_EVENT));
        event_client_uinit(evt_client);
        return NULL;
    }

    evt_client->msg_cache = (EVT_MSG_S *)malloc(sizeof(EVT_MSG_S) + EVT_DATA_SIZE);

    if (!evt_client->msg_cache)
    {
        printf("failed to alloc event client msg cache\n");
        event_client_uinit(evt_client);
        return NULL;
    }

    evt_client->running = TRUE;

    return evt_client;
}

int event_client_subscribe(EVT_CLIENT_S *evt_client, EVENT_TYPE_E evt_list[], UINT16_T evt_num)
{
    evt_client->msg_cache->module    = evt_client->module;
    evt_client->msg_cache->cmd       = EVT_CMD_SUBSCRIBE;
    evt_client->msg_cache->data_size = (sizeof(EVENT_TYPE_E) * evt_num);
    memcpy(evt_client->msg_cache->data, evt_list, evt_client->msg_cache->data_size);

    return pqueue_produce(evt_client->event_pq, evt_client->msg_cache, sizeof(EVT_MSG_S) + evt_client->msg_cache->data_size);
}

int event_client_unsubscribe(EVT_CLIENT_S *evt_client, EVENT_TYPE_E evt_list[], UINT16_T evt_num)
{
    evt_client->msg_cache->module    = evt_client->module;
    evt_client->msg_cache->cmd       = EVT_CMD_UNSUBSCRIBE;
    evt_client->msg_cache->data_size = (sizeof(EVENT_TYPE_E) * evt_num);
    memcpy(evt_client->msg_cache->data, evt_list, evt_client->msg_cache->data_size);

    return pqueue_produce(evt_client->event_pq, evt_client->msg_cache, sizeof(EVT_MSG_S) + evt_client->msg_cache->data_size);
}

int event_client_publish(EVT_CLIENT_S *evt_client, EVENT_TYPE_E evt_type, void *data, UINT16_T data_size)
{
    evt_client->msg_cache->module    = evt_client->module;
    evt_client->msg_cache->evt_type  = evt_type;
    evt_client->msg_cache->cmd       = EVT_CMD_PUBLISH;
    evt_client->msg_cache->data_size = data_size;
    memcpy(evt_client->msg_cache->data, data, data_size);

    return pqueue_produce(evt_client->event_pq, evt_client->msg_cache, sizeof(EVT_MSG_S) + data_size);
}

int event_client_work(EVT_CLIENT_S *evt_client)
{
    int ret = 0;

    ret = pthread_create(&evt_client->pid, NULL, event_client_loop, (void *)evt_client);

    if (0 > ret)
    {
        printf("failed to create event thread loop\n");
        return -1;
    }

    return 0;
}

int event_client_uinit(EVT_CLIENT_S *evt_client)
{
    if (evt_client)
    {
        evt_client->running = FALSE;

        pthread_join(evt_client->pid, NULL);

        pqueue_destory(evt_client->module_pq);

        pqueue_destory(evt_client->event_pq);

        if (evt_client->msg_cache)
        {
            free(evt_client->msg_cache);
        }

        free(evt_client);
    }

    return 0;
}