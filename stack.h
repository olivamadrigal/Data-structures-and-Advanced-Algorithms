/* stack ADT.
O(1) fast insert/delete.
pop/push from top only.
main functions: pop, push.
*/
#ifndef stack_h
#define stack_h
#include "wrappers.h"

typedef struct s_node
{
    void *dataPtr;
    struct s_node *next;
}SNODE;

typedef struct stack
{
    SNODE *top;
    void (*process)(void *data);
    int count;
}STACK;

STACK *create_stack_adt(void (*process)(void *data))
{
    STACK *s = (STACK*)Malloc(sizeof(STACK));
    s->top = NULL;
    s->process = process;
    s->count = 0;
    return s;
}

void push_stack_adt(STACK *s, void *dataIn)
{
    SNODE *pNew = (SNODE*)Malloc(sizeof(SNODE));
    
    pNew->dataPtr = dataIn;
    pNew->next = s->top;
    s->top = pNew;
    s->count++;
}


bool is_empty_stack_adt(STACK *s)
{
    return s->count == 0? true:false;
}

void *pop_stack_adt(STACK *s)
{
    SNODE *delNode;
    void *dataOut;
    
    delNode = s->top;
    dataOut = s->top->dataPtr;
    s->top = s->top->next;
    free(delNode);
    s->count--;
    return dataOut;
}

void destroy_stack_adt(STACK *s)
{
    SNODE *delNode;
    while(!is_empty_stack_adt(s))
    {
        delNode = s->top;
        s->top = s->top->next;
        free(delNode);
        s->count--;
    }
    free(s);
}


#endif /* stack_h */
