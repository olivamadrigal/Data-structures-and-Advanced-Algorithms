/*  linked list ADT with header and trailer nodes*/

#ifndef linked_list_hd_h
#define linked_list_hd_h
#include "wrappers.h"

typedef struct htll_node
{
    void *satellite;
    struct htll_node *next;
}HTLL_NODE;

typedef struct htll_head
{
    HTLL_NODE *front;
    HTLL_NODE *rear;
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    int count;
}HTLL_HEAD;

HTLL_HEAD *create_ht_linked_list_adt(int (*compare)(void *arg1, void *arg2),  void (*process)(void *data))
{
    HTLL_HEAD *head = (HTLL_HEAD*)Malloc(sizeof(HTLL_HEAD));
    head->front = NULL;
    head->rear = NULL;
    head->compare = compare;
    head->process = process;
    head->count = 0;
    return head;
}

/* O(n) worst */
bool _search_ht_linked_list_adt(HTLL_HEAD *list, HTLL_NODE **pre, HTLL_NODE **cur, void *target)
{
    bool found = false;
    *pre = NULL;
    
    //rear first.
    if(list->rear)
    {
        //if target is greater than the rear
        if(list->compare(target, list->rear->satellite) > 0)
        {
            *pre = list->rear;//update pre
            return found;
        }
    }
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
void insert_to_ht_linked_list_adt(HTLL_HEAD *list, void *data_in)
{
    HTLL_NODE *pNew, *pre, *cur;

    if(!_search_ht_linked_list_adt(list, &pre, &cur, data_in))
    {
        pNew = (HTLL_NODE*)Malloc(sizeof(HTLL_NODE));
        pNew->satellite = data_in;
        if(!pre)//empty list or inserting at front
        {
            pNew->next = list->front;
            list->front = pNew;
            if(list->count == 0)//set rear pointer
                list->rear = pNew;
        }
        else//middle or end
        {
            pNew->next = pre->next;
            pre->next = pNew;
            if(!pNew->next)//update rear if inserted at the end
                list->rear = pNew;
        }
        list->count++;
    }
    else
        DUPLICATE_ERROR;
}

/* O(1) +  search O(n) worst */
void delete_from_ht_linked_list_adt(HTLL_HEAD *list, void *target)
{
    HTLL_NODE *pre, *cur;
    
    if(_search_ht_linked_list_adt(list, &pre, &cur, target))
    {
        if(!pre)//delete at front
        {
            list->front = list->front->next;
            if(list->count == 1)//deleting only node
                list->rear = list->front = NULL;
        }
        else
        {
            pre->next = pre->next->next;
            if(!cur->next)//update rear
                list->rear = pre;
        }
        free(cur->satellite);
        free(cur);
        list->count--;
    }
    else
        DELETE_ERROR;
}

/* O(1) +  search O(n) worst */
void *retrive_from_ht_linked_list_adt(HTLL_HEAD *list, void *target)
{
    HTLL_NODE *pre, *cur;
    
    if(list->rear)
    {
        if(list->compare(target, list->rear->satellite) == 0)
            return list->rear->satellite;
    }
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
void traverse_ht_linked_list_adt(HTLL_HEAD *list)
{
    HTLL_NODE *mov = list->front;
    
    while(mov)
    {
        list->process(mov->satellite);
        mov =  mov->next;
    }
    
}

/* O(n) */
void delete_ht_linked_list_adt(HTLL_HEAD *list)
{
    HTLL_NODE *pre, *cur;
    
    cur = list->front;
    while(cur)
    {
        pre = cur;
        cur = cur->next;
        free(pre);
    }
    free(list);
}

int get_ht_linked_list_count(HTLL_HEAD *list)
{
    return list->count;
}



#endif /* linked_list_hd_h */
