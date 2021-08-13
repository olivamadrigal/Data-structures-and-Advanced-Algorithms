/* DOUBLY LINKED LIST ADT */

#ifndef d_linked_list_h
#define d_linked_list_h
#include "wrappers.h"

typedef struct dl_node
{
    void *satellite;
    struct dl_node *next;
    struct dl_node *back;
}DL_NODE;

typedef struct dl_head
{
    DL_NODE *front;
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    int count;
}DL_HEAD;

DL_HEAD *create_d_linked_list_adt(int (*compare)(void *arg1, void *arg2),  void (*process)(void *data))
{
    DL_HEAD *head = (DL_HEAD*)Malloc(sizeof(DL_HEAD));
    head->front = NULL;
    head->compare = compare;
    head->process = process;
    head->count = 0;
    return head;
}

/* O(n) worst */
bool _search_d_linked_list_adt(DL_HEAD *list, DL_NODE **pre, DL_NODE **cur, void *target)
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
void insert_to_d_linked_list_adt(DL_HEAD *list, void *data_in)
{
    DL_NODE *pNew, *pre, *cur;
    
    if(!_search_d_linked_list_adt(list, &pre, &cur, data_in))
    {
        pNew = (DL_NODE*)Malloc(sizeof(DL_NODE));
        pNew->satellite = data_in;
        if(!pre)//empty list or inserting at front
        {
                pNew->back = NULL;
                pNew->next = list->front;
                if(list->count > 0)
                    list->front->back = pNew;
                list->front = pNew;
        }
        else
        {
            if(cur)
            {
                pNew->next = pre->next;
                pre->next->back = pNew;
                pre->next = pNew;
                pNew->back = pre;
            }
            else //end
            {
                pNew->next = NULL;
                pNew->back = pre;
                pre->next = pNew;
            }
         
        }
        list->count++;
    }
    else
        DUPLICATE_ERROR;
}

/* O(1) +  search O(n) worst */
void delete_from_d_linked_list_adt(DL_HEAD *list, void *target)
{
    DL_NODE *pre, *cur;
    
    if(_search_d_linked_list_adt(list, &pre, &cur, target))
    {
        if(!pre)//delete at front
        {
            list->front = list->front->next;
            if(list->count > 1)
                list->front->back = NULL;
        }
        else
        {
            if(cur->next)//middle
            {
                pre->next = pre->next->next;
                pre->next->next->back = pre;
            }//last
            else
                pre->next = NULL;
        }
        free(cur);
        list->count--;
    }
    else
        DELETE_ERROR;
}

/* O(1) +  search O(n) worst */
void *retrive_from_d_linked_list_adt(DL_HEAD *list, void *target)
{
    DL_NODE *pre, *cur;
    
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
void traverse_d_linked_list_adt(DL_HEAD *list)
{
    DL_NODE *mov = list->front;
    while(mov)
    {
        list->process(mov->satellite);
        mov =  mov->next;
    }
    
}

/* O(n) */
void delete_d_linked_list_adt(DL_HEAD *list)
{
    DL_NODE *pre, *cur;
    
    cur = list->front;
    while(cur)
    {
        pre = cur;
        cur = cur->next;
        free(pre);
    }
    free(list);
}

int get_d_linked_list_count(DL_HEAD *list)
{
    return list->count;
}



#endif /* d_linked_list_h */
