#include <stdio.h>

#include "mlt_list.h"

void mlt_list_init(mlt_list_t *head)
{
    head->prev = head;
    head->next = head;
}

static void __mlt_list_add(mlt_list_t *new, mlt_list_t *prev, mlt_list_t *next)
{
    prev->next = new;
    next->prev = new;
    new->prev = prev;
    new->next = next;
}

void mlt_list_add(mlt_list_t *head, mlt_list_t *new)
{
    __mlt_list_add(new, head, head->next);
}

void mlt_list_add_tail(mlt_list_t *head, mlt_list_t *new)
{
    __mlt_list_add(new, head->prev, head);
}

static void __mlt_list_del(mlt_list_t *del, mlt_list_t *prev, mlt_list_t *next)
{
    prev->next = next;
    next->prev = prev;
}

void mlt_list_del(mlt_list_t *del)
{
    __mlt_list_del(del, del->prev, del->next);

    del->next = LIST_POISON1;
    del->prev = LIST_POISON2;
}