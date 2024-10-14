#include <stdio.h>

#include "event.h"

static int event_server_test(void)
{
    EVT_SERVER_S *evt_server = NULL;

    evt_server = event_server_init();

    if (!evt_server)
    {
        printf("failed to init event server\n");
        return -1;
    }

    event_server_work(evt_server);

    fgetc(stdin);

    event_server_uinit(evt_server);

    return 0;
}

static int event_client_handle_test(EVENT_TYPE_E evt_type, void *data, UINT16_T data_size)
{
    printf("event 0x%x data %d\n", evt_type, *(int *)data);

    return 0;
}

static int event_client_test(int flag)
{
    int data = 0;
    EVT_CLIENT_S *evt_client = NULL;

    if (flag)
    {
        EVENT_TYPE_E  sub_evt_list[] = {
            EVT_THREADPOOL_POP,
            EVT_THREADPOOL_PUSH,
            EVT_THREADPOOL_WORK
        };

        evt_client = event_client_init(SDP_MD_BOARDINFO, event_client_handle_test);

        if (!evt_client)
        {
            printf("failed to init event client\n");
            return 0;
        }

        event_client_subscribe(evt_client, sub_evt_list, ITEM(sub_evt_list));

        event_client_work(evt_client);

        fgetc(stdin);

        data++;
        event_client_publish(evt_client, EVT_BOARD_CPU_FREQ, (void *)(&data), sizeof(data));

        fgetc(stdin);

        data++;
        event_client_publish(evt_client, EVT_BOARD_CPU_TEMP, (void *)(&data), sizeof(data));

        fgetc(stdin);

        data++;
        event_client_publish(evt_client, EVT_BOARD_CPU_USAGE, (void *)(&data), sizeof(data));

        fgetc(stdin);

        event_client_unsubscribe(evt_client, sub_evt_list, ITEM(sub_evt_list));

        fgetc(stdin);

        event_client_uinit(evt_client);
    }
    else
    {
        EVENT_TYPE_E sub_evt_list[] = {
            EVT_BOARD_CPU_FREQ,
            EVT_BOARD_CPU_TEMP,
            EVT_BOARD_CPU_USAGE
        };

        evt_client = event_client_init(SDP_MD_THREADPOOL, event_client_handle_test);

        if (!evt_client)
        {
            printf("failed to init event client\n");
            return 0;
        }

        event_client_subscribe(evt_client, sub_evt_list, ITEM(sub_evt_list));

        event_client_work(evt_client);

        fgetc(stdin);

        data++;
        event_client_publish(evt_client, EVT_THREADPOOL_POP, (void *)(&data), sizeof(data));

        fgetc(stdin);

        data++;
        event_client_publish(evt_client, EVT_THREADPOOL_PUSH, (void *)(&data), sizeof(data));

        fgetc(stdin);

        data++;
        event_client_publish(evt_client, EVT_THREADPOOL_WORK, (void *)(&data), sizeof(data));

        fgetc(stdin);

        event_client_unsubscribe(evt_client, sub_evt_list, ITEM(sub_evt_list));

        fgetc(stdin);

        event_client_uinit(evt_client);
    }

    return 0;
}

int main(int argc, char *argv[ ])
{
    if (1 == argc)
    {
        event_server_test();
    }
    else
    {
        event_client_test((2 < argc));
    }

    return 0;
}