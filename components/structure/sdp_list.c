#include <stdio.h>

#include "sdp_list.h"

void sdp_list_init(sdp_list_t *head)
{
    head->prev = head;
    head->next = head;
}

static void __sdp_list_add(sdp_list_t *new, sdp_list_t *prev, sdp_list_t *next)
{
    prev->next = new;
    next->prev = new;
    new->prev = prev;
    new->next = next;
}

void sdp_list_add(sdp_list_t *head, sdp_list_t *new)
{
    __sdp_list_add(new, head, head->next);
}

void sdp_list_add_tail(sdp_list_t *head, sdp_list_t *new)
{
    __sdp_list_add(new, head->prev, head);
}

static void __sdp_list_del(sdp_list_t *del, sdp_list_t *prev, sdp_list_t *next)
{
    prev->next = next;
    next->prev = prev;
}

void sdp_list_del(sdp_list_t *del)
{
    __sdp_list_del(del, del->prev, del->next);

    del->next = LIST_POISON1;
    del->prev = LIST_POISON2;
}