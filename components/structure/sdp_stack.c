#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdp_stack.h"

#define PTR_CHECK(p) {  \
    if (!p) {           \
        return -1;      \
    }                   \
}

SDP_STACK_T *sdp_stack_create(int node_num, int node_size)
{
    SDP_STACK_T *stack = NULL;

    stack = (SDP_STACK_T *)malloc(sizeof(SDP_STACK_T));

    if (!stack) 
    {
        return NULL;
    }

    stack->addr = malloc(node_num * node_size);
    if (!stack->addr) 
    {
        return NULL;
    }

    memset(stack->addr, 0, node_num * node_size);

    stack->top = 0;
    stack->prod = 0;
    stack->total = node_num;
    stack->avail = node_num;
    stack->node_size = node_size;

    return stack;
}

int sdp_enstack(SDP_STACK_T *stack, void *node, int size)
{
    PTR_CHECK(stack);
    PTR_CHECK(node);
    PTR_CHECK(stack->addr);

    void *prod_addr = NULL;

    if (size > stack->node_size) 
    {
        return -2;
    }

    if (!stack->avail) 
    {
        return -3;
    }

    if (stack->prod >= stack->total)
    {
        stack->prod = 0;
    }

    if (stack->top >= stack->total)
    {
        stack->top = 0;
    }

    prod_addr = stack->addr + (stack->prod * stack->node_size);

    memcpy(prod_addr, node, size);

    stack->top = stack->prod++;
    stack->avail--;

    return 0;
}

int sdp_destack(SDP_STACK_T *stack, void *node, int size)
{
    PTR_CHECK(stack);
    PTR_CHECK(node);
    PTR_CHECK(stack->addr);

    void *top_addr = NULL;

    if (stack->avail == stack->total) 
    {
        return -4;
    }

    if (stack->node_size < size) 
    {
        return -5;
    }

    if (stack->top < 0) 
    {
        stack->top = 0;
    }

    if (stack->prod < 0)
    {
        stack->prod = 0;
    }

    top_addr = stack->addr + (stack->top * stack->node_size);

    memcpy(node, top_addr, size);
    
    stack->prod--;
    stack->top--;
    
    stack->avail++;

    return 0;
}

int sdp_stack_top(SDP_STACK_T *stack, void *node, int size)
{
    PTR_CHECK(stack);
    PTR_CHECK(node);
    PTR_CHECK(stack->addr);

    void *top_addr = NULL;

    if (stack->avail == stack->total) 
    {
        return -4;
    }

    if (stack->node_size < size) 
    {
        return -5;
    }

    top_addr = stack->addr + (stack->top * stack->node_size);

    memcpy(node, top_addr, size);
    
    return 0;
}

void sdp_stack_free(SDP_STACK_T *stack)
{
    if (stack) 
    {
        if (stack->addr) 
        {
            free(stack->addr);
            stack->addr = NULL;
        }

        stack->prod = 0;
        stack->top = 0;
        stack->total = 0;
        stack->avail = 0;
        stack->node_size = 0;

        free(stack);
    }
}

#if 0
int main()
{
    SDP_STACK_T *stack = sdp_stack_create(100, sizeof(int));

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

