#ifndef MLT_LIST
#define MLT_LIST

#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200200)

typedef struct mlt_list_head
{
    struct mlt_list_head *prev;
    struct mlt_list_head *next;
} mlt_list_t;

void mlt_list_init(mlt_list_t *head);

void mlt_list_add(mlt_list_t *head, mlt_list_t *new);

void mlt_list_add_tail(mlt_list_t *head, mlt_list_t *new);

void mlt_list_del(mlt_list_t *del);

#define offsetof(type, member) ((size_t) &(((type *)0)->member))


#define container_of(list, type, member)                    \
({                                                          \
    const typeof(((type *)0)->member)*_ptr = (list);        \
    (type *)((char *)_ptr - offsetof(type, member));        \
})


#define mlt_list_entry(list, type, member)                  \
    container_of(list, type, member)


#define mlt_list_for_each_entry(pos, head, member)                         \
    for (pos = mlt_list_entry((head)->next, typeof(*pos), member);         \
         &pos->member != (head);                                            \
         pos = mlt_list_entry(pos->member.next, typeof(*pos), member))


#define mlt_list_for_each_entry_safe(pos, next, head, member)              \
    for (pos = mlt_list_entry((head)->next, typeof(*pos), member),         \
         next = mlt_list_entry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head);                                            \
         pos = next, next = mlt_list_entry(next->member.next, typeof(*pos), member))

#define mlt_LIST_EMPTY(list) ((list)->prev == (list) && (list)->next == (list))


#endif
