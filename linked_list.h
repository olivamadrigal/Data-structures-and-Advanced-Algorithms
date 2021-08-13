/*  linked list ADT
    different ways to assign pointers... play with them. */

#ifndef linked_list_h
#define linked_list_h
#include "wrappers.h"

typedef struct ll_node
{
    void *satellite;
    struct ll_node *next;
}LL_NODE;

typedef struct ll_head
{
    LL_NODE *front;
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    int count;
}LL_HEAD;

LL_HEAD *create_linked_list_adt(int (*compare)(void *arg1, void *arg2),  void (*process)(void *data))
{
    LL_HEAD *head = (LL_HEAD*)Malloc(sizeof(LL_HEAD));
    head->front = NULL;
    head->compare = compare;
    head->process = process;
    head->count = 0;
    return head;
}

/* O(n) worst */
bool _search_linked_list_adt(LL_HEAD *list, LL_NODE **pre, LL_NODE **cur, void *target)
{
    bool found = false;
    *pre = NULL;
    if(list->front)
    {
        *cur = list->front;
        while(*cur && (list->compare(target, (*cur)->satellite) > 0 ))
        {
            *pre = *cur;
            *cur = (*cur)->next;
        }
        if(*cur &&  (list->compare(target, (*cur)->satellite) == 0))
            found = true;
    }
    return found;
}

/* O(1) +  search O(n) worst */
void insert_to_linked_list_adt(LL_HEAD *list, void *data_in)
{
    LL_NODE *pNew, *pre, *cur;
    
    if(!_search_linked_list_adt(list, &pre, &cur, data_in))
    {
        pNew = (LL_NODE*)Malloc(sizeof(LL_NODE));
        pNew->satellite = data_in;
        if(!pre)//empty list or inserting at front
        {
            pNew->next = list->front;
            list->front = pNew;
        }
        else//middle or end
        {
            pNew->next = pre->next;
            pre->next = pNew;
        }
        list->count++;
    }
    else
        DUPLICATE_ERROR;
}

/* O(1) +  search O(n) worst */
void delete_from_linked_list_adt(LL_HEAD *list, void *target)
{
    LL_NODE *pre, *cur;
    
    if(_search_linked_list_adt(list, &pre, &cur, target))
    {
        if(!pre)//delete at front
            list->front = list->front->next;
        else//middle or last
            pre->next = pre->next->next;
        free(cur);
        list->count--;
    }
    else
        DELETE_ERROR;
}

/* O(1) +  search O(n) worst */
void *retrive_from_linked_list_adt(LL_HEAD *list, void *target)
{
    LL_NODE *pre, *cur;
    
    if(list->front)
    {
        cur = list->front;
        while(cur && (list->compare(target, cur->satellite) > 0 ))
        {
            pre = cur;
            cur = (cur)->next;
        }
        if(cur &&  (list->compare(target, cur->satellite) == 0))
           return cur->satellite;
    }
    NOT_FOUND_ERROR;
    return NULL;
}

/* O(n) */
void traverse_linked_list_adt(LL_HEAD *list)
{
    LL_NODE *mov = list->front;
    
    while(mov)
    {
        list->process(mov->satellite);
        mov =  mov->next;
    }
}

/* O(n) */
void delete_linked_list_adt(LL_HEAD *list)
{
    LL_NODE *pre, *cur;
    
    cur = list->front;
    while(cur)
    {
        pre = cur;
        cur = cur->next;
        free(pre);
    }
    free(list);
}

int get_linked_list_count(LL_HEAD *list)
{
    return list->count;
}


#endif /* linked_list_h */
