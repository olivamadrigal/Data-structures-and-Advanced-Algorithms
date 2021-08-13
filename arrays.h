/*
    Fixed-size arrays are useful when we know the size of the problem and require fast random access.
    The alternative is pointer-based data structures.
    Dynamic arrays which can shrink and grow at run time are risky: can create run time error if
    you malloc attempt runs into space that has already been allocated.
    assume: realloc  = {malloc new size, memcpy old data to new space, followed by free of old space}.
*/

#ifndef arrays_h
#define arrays_h
#include "wrappers.h"

typedef struct ary_header
{
    void (*process)(void *dataPtr);
    int (*compare)(void *arg1, void *arg2);
    void **ary;
    int size;
    int count;
    
}ARY_HEADER;

ARY_HEADER *create_array_adt(int size, void (*process)(void *data), int (*compare)(void *arg1, void *arg2));
void search_array_adt(ARY_HEADER *header, void *target, int *idx, bool *found);
bool is_array_full(ARY_HEADER *header);
/* insert cost has worst O(n) due to search*/
void insert_to_array_adt(ARY_HEADER *header, void *dataIn);
/* delete cost has worst O(n) due to search*/
bool delete_from_array_adt(ARY_HEADER *header, void *target);
/* FAST 0(1) random access or retrieval*/
void *retrieve_from_array_adt(ARY_HEADER *header, int idx);
/*O(n) */
void traverse_array_adt(ARY_HEADER *header);
/*O(1) */
void destroy_array_adt(ARY_HEADER *header);

ARY_HEADER *create_array_adt(int size, void (*process)(void *data), int (*compare)(void *arg1, void *arg2))
{
    ARY_HEADER* header;
    
    header = (ARY_HEADER*)Malloc(sizeof(ARY_HEADER));
    header->ary = (void**)Calloc(size, sizeof(void*));
    header->compare = compare;
    header->process = process;
    header->size = size;
    header->count = 0;
    return header;
}

void search_array_adt(ARY_HEADER *header, void *target, int *idx, bool *found)
{
    int i;
    *found = false;
    for(i = 0; i < header->count && header->compare(target, header->ary[i]) > 0;  i++)
        ;
    if(i < header->count) // if target is before first or in the middle
    {
        if(header->compare(target, header->ary[i]) == 0)
            *found = true;
    }
    *idx = i;
}

//

// [5]
//  0
// target = [2]
// idx 0
// [5][7][9] = count = 3
//  0  1  2
// target = [2]
// [5][7][9]
//  0  1 2
// target = [8]

bool is_array_full(ARY_HEADER *header)
{
    if(header->count == header->size)
        return true;
    else
        return false;
}

/* insert cost has worst O(n) due to search*/
void insert_to_array_adt(ARY_HEADER *header, void *dataIn)
{
    bool found;
    int idx, u, i;
    
    
    if(header->count == 0)
    {
        header->ary[0] = dataIn;
        header->count++;
    }
    else
    {
        if(!is_array_full(header))
        {
            search_array_adt(header, dataIn, &idx, &found);
            if(!found)
            {
                if(!header->ary[idx])
                    header->ary[idx] = dataIn;
                else
                {
                    //shift elements up
                    for(u = header->count, i = header->count - 1; i >= idx; u--, i--)
                            header->ary[u] = header->ary[i];
                    header->ary[idx] = dataIn;
                }
            header->count++;
            }
        }//search
    }
    
}

/* delete cost has worst O(n) due to search*/
bool delete_from_array_adt(ARY_HEADER *header, void *target)
{
    bool found;
    int idx, d, i;
    
    search_array_adt(header, target, &idx, &found);
    if(found)
    {
        //shift elements down
        for(i = idx, d = idx + 1; d <= header->count; i++, d++)
            header->ary[i] = header->ary[d];
        //header->ary[header->count-1] = NULL;
        header->count--;
    }
    return found;
}

/* FAST 0(1) random access or retrieval*/
void *retrieve_from_array_adt(ARY_HEADER *header, int idx)
{
    if(idx >= header->size)
    {
        INDEX_ERROR;
        return NULL;
    }
    if(header->ary[idx])
        return header->ary[idx];
    else
        return NULL;
}


/*O(n) */
void traverse_array_adt(ARY_HEADER *header)
{
    for(int i = 0; i < header->count; i++)
        header->process(header->ary[i]);
}

/*O(1) */
void destroy_array_adt(ARY_HEADER *header)
{
    free(header->ary);
    free(header);
}
#endif /* arrays_h */

