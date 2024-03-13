#ifndef SDP_LIST
#define SDP_LIST

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

typedef struct sdp_list_head
{
    struct sdp_list_head *prev;
    struct sdp_list_head *next;
} sdp_list_t;

void sdp_list_init(sdp_list_t *head);

void sdp_list_add(sdp_list_t *head, sdp_list_t *new);

void sdp_list_add_tail(sdp_list_t *head, sdp_list_t *new);

void sdp_list_del(sdp_list_t *del);

#define offsetof(type, member) ((size_t) &(((type *)0)->member))


#define container_of(list, type, member)                    \
({                                                          \
    const typeof(((type *)0)->member)*_ptr = (list);        \
    (type *)((char *)_ptr - offsetof(type, member));        \
})


#define sdp_list_entry(list, type, member)                  \
    container_of(list, type, member)


#define sdp_list_for_each_entry(pos, head, member)                         \
    for (pos = sdp_list_entry((head)->next, typeof(*pos), member);         \
         &pos->member != (head);                                            \
         pos = sdp_list_entry(pos->member.next, typeof(*pos), member))


#define sdp_list_for_each_entry_safe(pos, next, head, member)              \
    for (pos = sdp_list_entry((head)->next, typeof(*pos), member),         \
         next = sdp_list_entry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head);                                            \
         pos = next, next = sdp_list_entry(next->member.next, typeof(*pos), member))

#define sdp_LIST_EMPTY(list) ((list)->prev == (list) && (list)->next == (list))


#endif
