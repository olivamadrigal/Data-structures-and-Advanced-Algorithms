#ifndef dynamic_arrays_h
#define dynamic_arrays_h
#include "wrappers.h"

typedef struct dary_header
{
    void (*process)(void *dataPtr);
    int (*compare)(void *arg1, void *arg2);
    void **ary;
    int size;
    int count;
    
}DARY_HEADER;


DARY_HEADER *create_darray_adt(int size, void (*process)(void *data), int (*compare)(void *arg1, void *arg2))
{
    DARY_HEADER* header;
    
    header = (DARY_HEADER*)Malloc(sizeof(DARY_HEADER));
    header->ary = (void**)Calloc(size, sizeof(void*));
    header->compare = compare;
    header->process = process;
    header->size = size;
    header->count = 0;
    return header;
}

void search_darray_adt(DARY_HEADER *header, void *target, int *idx, bool *found)
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

bool is_darray_full(DARY_HEADER *header)
{
    if(header->count == header->size)
        return true;
    else
        return false;
}

/* insert cost has worst O(n) due to search*/
void insert_to_darray_adt(DARY_HEADER *header, void *dataIn)
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
        if(is_darray_full(header))//duplicate the size each time it gets full
        {
            puts("\nREALLOC\n");
            header->size = 2*header->size;
            Realloc(header->ary, sizeof(void*)*header->size);//realloc with new size
        }
        search_darray_adt(header, dataIn, &idx, &found);
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

    }
    
}

/* delete cost has worst O(n) due to search*/
bool delete_from_darray_adt(DARY_HEADER *header, void *target)
{
    bool found;
    int idx, d, i;
    
    search_darray_adt(header, target, &idx, &found);
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
void *retrieve_from_darray_adt(DARY_HEADER *header, int idx)
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
void traverse_darray_adt(DARY_HEADER *header)
{
    for(int i = 0; i < header->count; i++)
        header->process(header->ary[i]);
}

/*O(1) */
void destroy_darray_adt(DARY_HEADER *header)
{
    //free(header->ary);
    //free(header);
}

#endif /* dynamic_arrays_h */
