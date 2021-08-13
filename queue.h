#ifndef queue_h
#define queue_h
#include "wrappers.h"

/*queue adt:
conceptually: insert at rear, remove from front
implementation: reverse the "header" , "trailer"" linked list
MAIN FUNCTIONS: enqueue, dequeue, + other utility functions: print, is_empty, get_count
queue is specific to problem, main use case is FAST O(1) ADD/DEL
*/

typedef struct q_node
{
    void *dataPtr;
    struct q_node *next;
    
}QNODE;

typedef struct queue
{
    void (*process)(void *data);
    QNODE *front;
    QNODE *rear;
    int count;
}QUEUE;


QUEUE *create_queue_adt(void (*process)(void *data))
{
    QUEUE *q = (QUEUE*)Malloc(sizeof(QUEUE));
    
    q->process = process;
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
    
    return q;
}

/* O(1) */
void enqueue_queue_adt(QUEUE *q, void *dataIn)
{
    QNODE *pNew = (QNODE*)Malloc(sizeof(QNODE));
    
    pNew->dataPtr = dataIn;
    if(q->count == 0)
    {
        q->front = pNew;
        q->rear = pNew;
    }
    else{
        q->rear->next = pNew;
        q->rear = pNew;
    }
    //q->rear = pNew;
    pNew->next = NULL;
    q->count++;
}

/* O(1) */
void *deque_queue_adt(QUEUE *q)
{
    QNODE *delPtr;
    void  *dataOut;
    delPtr = q->front;
    dataOut = q->front->dataPtr;
    if(q->count == 1)
        q->front = q->rear = NULL;
    else
        q->front = q->front->next;
    free(delPtr);
    q->count--;
    return dataOut;
}

bool is_queue_empty_queue_adt(QUEUE *q)
{
    return (q->count == 0)?true:false;
}

int get_count_queue_adt(QUEUE *q)
{
    return q->count;
}

void print_queue_adt(QUEUE *q)
{
    QNODE *mov = q->front;
    
    while(mov)
    {
        q->process(mov->dataPtr);
        mov = mov->next;
    }
}

void delete_queue_adt(QUEUE *q)
{
    QNODE *pre, *cur;
    
    cur = q->front;
    while(cur)
    {
        pre = cur;
        cur = cur->next;
        free(pre);
    }
    free(q);
}
#endif 
