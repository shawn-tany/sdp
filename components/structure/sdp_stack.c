#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdp_stack.h"

#define PTR_CHECK(p) {  \
    if (!p) {           \
        return -1;      \
    }                   \
}

sdp_stack_t *sdp_stack_create(int node_num, int node_size)
{
    sdp_stack_t *stack = NULL;

    stack = (sdp_stack_t *)malloc(sizeof(sdp_stack_t));

    if (!stack) {
        return NULL;
    }

    stack->addr = malloc(node_num * node_size);

    if (!stack->addr) {
        return NULL;
    }

    memset(stack->addr, 0, node_num * node_size);

    stack->top = stack->addr;
    stack->prod = stack->addr;

    stack->total = node_num;
    stack->avail = node_num;
    stack->offset = node_size;

    return stack;
}

int sdp_enstack(sdp_stack_t *stack, void *node, int size)
{
    PTR_CHECK(stack);
    PTR_CHECK(node);
    PTR_CHECK(stack->addr);

    if (size > stack->offset) {
        return -2;
    }

    if (!stack->avail) {
        return -3;
    }

    memcpy(stack->prod, node, size);

    if (stack->prod > stack->addr + (stack->offset * stack->total) ||
        stack->prod + stack->offset > stack->addr + (stack->offset * stack->total)) {
        
        stack->prod = stack->addr;
    }

    stack->prod = stack->prod + stack->offset;
    
    stack->avail--;

    return 0;
}

int sdp_destack(sdp_stack_t *stack, void *node, int size)
{
    PTR_CHECK(stack);
    PTR_CHECK(node);
    PTR_CHECK(stack->addr);

    if (stack->avail == stack->total) {
        return -4;
    }

    if (stack->offset < size) {
        return -5;
    }

    stack->top = stack->prod - stack->offset;

    if (stack->top < stack->addr) {
        stack->prod = stack->addr + ((stack->total - 1) * stack->offset);
    }

    memcpy(node, stack->top, size);
    
    stack->prod = stack->prod - stack->offset;
    
    stack->avail++;

    return 0;
}

int sdp_stack_top(sdp_stack_t *stack, void *node, int size)
{
    PTR_CHECK(stack);
    PTR_CHECK(node);
    PTR_CHECK(stack->addr);

    if (stack->avail == stack->total) {
        return -4;
    }

    if (stack->offset < size) {
        return -5;
    }

    void *top = NULL;

    top = stack->prod - stack->offset;

    memcpy(node, top, size);
    
    return 0;
}

void sdp_stack_free(sdp_stack_t *stack)
{
    if (stack) {
        if (stack->addr) {
            free(stack->addr);
            stack->addr = NULL;
        }

        stack->prod = NULL;
        stack->top = NULL;
        
        stack->total = 0;
        stack->avail = 0;
        stack->offset = 0;

        free(stack);
    }
}

#if 0
int main()
{
    sdp_stack_t *stack = sdp_stack_create(100, sizeof(int));

    if (!stack) {
        printf("faild to create stack\n");
        return -1;
    }

    int i;
    int node;
    int idex = 0;

    int start = 0;
    int loop = 0;

    loop = start + 50;
    for (i = start; i < loop; ++i, ++start) {
        if (sdp_enstack(stack, &i, sizeof(i))) {
            printf("failed to enstack\n");
            goto FAIL;
        }
    }

    while (!sdp_destack(stack, &node, sizeof(node))) {
        printf("destack[%d] : %d\n", idex++, node);
    }

    loop = start + 100;
    for (i = start; i < loop; ++i, ++start) {
        if (sdp_enstack(stack, &i, sizeof(i))) {
            printf("failed to enstack\n");
            goto FAIL;
        }
    }

    while (!sdp_destack(stack, &node, sizeof(node))) {
        printf("destack[%d] : %d\n", idex++, node);
    }

    loop = start + 100;
    for (i = start; i < loop; ++i, ++start) {
        if (sdp_enstack(stack, &i, sizeof(i))) {
            printf("failed to enstack\n");
            goto FAIL;
        }
    }

    while (!sdp_destack(stack, &node, sizeof(node))) {
        printf("destack[%d] : %d\n", idex++, node);
    }

    sdp_stack_free(stack);

    return 0;

FAIL :

    sdp_stack_free(stack);

    return -1;
}
#endif

