#ifndef c_linked_list_h
#define c_linked_list_h

/*  circular linked list ADT
    last node has pointer to first node.
*/
#include "wrappers.h"

typedef struct cll_node
{
    void *satellite;
    struct cll_node *next;
}CLL_NODE;

typedef struct cll_head
{
    CLL_NODE *front;
    CLL_NODE *last;
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    int count;
}CLL_HEAD;

int get_c_linked_list_count(CLL_HEAD *list)
{
    return list->count;
}

CLL_HEAD *create_c_linked_list_adt(int (*compare)(void *arg1, void *arg2),  void (*process)(void *data))
{
    CLL_HEAD *head = (CLL_HEAD*)Malloc(sizeof(CLL_HEAD));
    head->front = NULL;
    head->last = NULL;
    head->compare = compare;
    head->process = process;
    head->count = 0;
    return head;
}

/* O(n) worst */
bool _search_c_linked_list_adt(CLL_HEAD *list, CLL_NODE **pre, CLL_NODE **cur, void *target)
{
    bool found;
    int res;
    
    found = false;
    *pre = NULL;
    
    if(list->front)
    {
        *cur = list->front;
        if(list->count == 1)//if only one element
        {
            res = list->compare(target, (*cur)->satellite);
            if(res == 0)
                found = true;
            else
                if(res > 0)
                    *pre = *cur;
        }
        else
        {   //search rest of list
            while(((*cur)->next != list->front) && (list->compare(target, (*cur)->satellite) > 0 ))
            {
                *pre = *cur;
                *cur = (*cur)->next;
            }
            res = list->compare(target, (*cur)->satellite);
            if( res == 0)
                found = true;
            else
                if(res > 0)
                    *pre = *cur;
        }
    }
    return found;
}

/* O(1) +  search O(n) worst */
void insert_to_c_linked_list_adt(CLL_HEAD *list, void *data_in)
{
    CLL_NODE *pNew, *pre, *cur;
    
    if(!_search_c_linked_list_adt(list, &pre, &cur, data_in))
    {
        pNew = (CLL_NODE*)Malloc(sizeof(CLL_NODE));
        pNew->satellite = data_in;
        if(!pre)//empty list or inserting at front
        {
            if(list->count == 0)
            {
                list->front = pNew;
                list->last = pNew;
                pNew->next = list->front;//creates circle

            }
            else
            {
                pNew->next = list->front;
                list->front = pNew;
                list->last->next = list->front;
            }
        }
        else
        {
            if(pre->next != list->front)//in middle
            {
                pNew->next = pre->next;
                pre->next = pNew;
            }
            else//at the end
            {
                pre->next = pNew;
                list->last = pNew;
                pNew->next = list->front;
            }
        }
        list->count++;
    }
    else
        DUPLICATE_ERROR;
}

/* O(1) +  search O(n) worst */
void delete_from_c_linked_list_adt(CLL_HEAD *list, void *target)
{
    CLL_NODE *pre, *cur;
    
    if(_search_c_linked_list_adt(list, &pre, &cur, target))
    {
        if(!pre)//delete at front
        {
            if(list->count == 1)
            {
                list->front = NULL;
                list->last = NULL;
            }
            else
            {
                list->front = list->front->next;//
                list->last->next = list->front;//update this ptr
            }
        }
        else if((pre->next != list->front))//in the middle
            pre->next = pre->next->next;
        else//at the end
        {
            list->last = pre;
            list->last->next = list->front;
        }
        free(cur);
        list->count--;
    }
    else
        DELETE_ERROR;
}

/* O(1) +  search O(n) worst */
void *retrive_from_c_linked_list_adt(CLL_HEAD *list, void *target)
{
    CLL_NODE *pre, *cur;
    
    if(list->front)
    {
        cur = list->front;
        //search rest of list
        while((cur->next != list->front) && (list->compare(target, cur->satellite) > 0 ))
        {
                pre = cur;
                cur = cur->next;
        }
        if(cur &&  (list->compare(target, cur->satellite) == 0))
            return cur->satellite;
    }
    NOT_FOUND_ERROR;
    return NULL;
}

/* O(n) */
void traverse_c_linked_list_adt(CLL_HEAD *list)
{
    
    CLL_NODE *mov = list->front;
    /*
    while(mov->next != list->front) //count > 0)//while we are not back at the start
    {
        printf("%p:%p\n", mov, mov->next);
        list->process(mov->satellite);
        mov =  mov->next;
    }
    printf("%p:%p\n", mov, mov->next);
    list->process(mov->satellite);
    */
    int count = get_c_linked_list_count(list);
    while(count > 0)
    {
        printf("%p:%p\n", mov, mov->next);
        list->process(mov->satellite);
        mov =  mov->next;
        count--;
    }

}

/* O(n) */
void delete_c_linked_list_adt(CLL_HEAD *list)
{
    CLL_NODE *pre, *cur;
    
    cur = list->front;
    while(cur->next != list->front)
    {
        pre = cur;
        cur = cur->next;
        puts("DELETING:");
        list->process(pre->satellite);
        free(pre);
    }
    puts("DELETING:");
    list->process(cur->satellite);
    free(cur);
    free(list);
}

#endif /* c_linked_list_h */
