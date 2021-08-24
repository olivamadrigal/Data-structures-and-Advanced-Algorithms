#ifndef heap_h
#define heap_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "queue.h"
#include "wrappers.h"

//HEAP ADT
//****GENERIC HEAP ADT****/
typedef struct heap_adt
{
    int (*compare)(void *arg1, void *arg2);
    int size;
    int count;
    void **ary;
    
}HEAP_ADT; //fixed-size static array heap

void heap_insert_adt(HEAP_ADT *heap, void *data_in);
HEAP_ADT* build_heap_adt(int size, int (*compare)(void *arg1, void *arg2));
void* get_min(HEAP_ADT *heap);
void reheap_down_min_adt(HEAP_ADT *heap, int parent);
void reheap_up_min_adt(HEAP_ADT *heap, int child_idx);
void min_heapify_adt(HEAP_ADT *heap, int parent_idx);
void build_min_heap(HEAP_ADT* heap);
void destroy_heap_adt(HEAP_ADT *heap);

//APPLICATION-SPECIFIC HEAP with array of INTR structures
typedef struct intr
{
    int priority;
    char *description;
}INTR;

typedef struct aheap
{
    int max_size;
    int size;
    INTR *intr_vector;//array of structs per se.    
}AHEAP; //fixed-size static array heap

//HEAP ADT with DYNAMIC ARRAY
typedef struct dheap
{
    int max_size;
    int realloc_amt;//by how much), new_size = (old_size + realloc_amt)
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    void **ary; //DYNAMIC ARRAY of PTRS TO VOID
    int count;
    int last;
}DHEAP;

//ADT POINTER-BASED HEAP
//for kicks but has applications.
//draw out like a mesh and figure out how to move around with ptrs.
typedef struct heap_node
{
    void *data;
    struct heap_node *left;
    struct heap_node *right;
    struct heap_node *parent;
    struct heap_node *left_neighbor;
    struct heap_node *right_neighbor;
}HNODE;

typedef struct level
{
    int n;
    bool full;
    int count;
}LEVEL;

typedef struct pheap
{
    int max_size;
    LEVEL *ary;
    int current_level;
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    void (*swap)(void *arg1, void *arg2);
    HNODE *root;
    HNODE *last;//LEFT MOST LAST LEAF
    int count;
}PHEAP; //node, pointer based, heap

typedef enum {MAX=1, MIN} HEAP_TYPE;
typedef enum {FIXED_ARRAY=1, DYNAMIC_ARRAY=2, POINTER_BASED=3} IMPLEMENTATION_TYPE;

/* ARRAY-based heap which is the most natural implementation used in nearly all cases */
/**** INTR fixed-size array *****/
AHEAP* create_array_heap(int size, HEAP_TYPE htype, IMPLEMENTATION_TYPE itype);
void build_heap(AHEAP *header, HEAP_TYPE htype);
void max_heapify(AHEAP *heap, int parent_idx);
void print_heap(AHEAP *heap);
INTR get_root(AHEAP *heap, HEAP_TYPE htype);
void reheap_down(AHEAP *heap, int parent);
void heap_insert(AHEAP *heap, INTR data_in, HEAP_TYPE type);
void reheap_up(AHEAP *heap, int child_idx);
void min_heapify(AHEAP *heap, int parent_idx);
void reheap_down_min(AHEAP *heap, int parent);
void reheap_up_min(AHEAP *heap, int child_idx);

/***** DYNAMIC ARRAY HEAP -- USE MAX HEAP -- INTERGER-SPECIFC *****/
DHEAP* build_dynamic_array_heap_adt(int size, int inc, HEAP_TYPE htype, IMPLEMENTATION_TYPE itype,
                                    int (*compare)(void *arg1, void *arg2), void (*process)(void *data));
bool dheap_insert(DHEAP *heap, void *data_in);
void _dreheap_up(DHEAP *heap, int child_idx);
void _dreheap_down(DHEAP *heap, int parent_idx);
void print_heap_ary(DHEAP *heap);
void get_dheap_stats(DHEAP *heap);
void destroy_dynamic_heap_array(DHEAP *heap);

/***** ADT POINTER-BASED HEAP -- USE MAX HEAP ****/
PHEAP* create_pointer_based_heap(int size, int (*compare)(void *arg1, void *arg2), void (*process)(void *data), void (*swap)(void *arg1, void *arg2));
HNODE* _preheap_up(PHEAP *heap, HNODE *root, HNODE *new_node);
void print_pheap_ary(PHEAP *heap);
void heap_breath_first_insertion(PHEAP *heap, HNODE *root, HNODE *child);
void bft_verify_max_heap(PHEAP *heap, HNODE *root, HNODE *child);
void pointer_based_heap_breath_first_traversal(PHEAP *root);
LEVEL* set_levels_array(int n);
bool pointer_based_heap_insert(PHEAP *heap, void *data_in);
bool level_i_full(LEVEL *ary, int i);
void pointer_based_heap_get_root(PHEAP *heap);
bool pointer_based_heap_delete(PHEAP *heap);
HNODE* heap_breath_first_last_node(PHEAP *tree);
void destroy_pointer_based_heap(PHEAP *heap);


AHEAP* create_array_heap(int size, HEAP_TYPE htype, IMPLEMENTATION_TYPE itype)
{
    AHEAP *header;
    
    header = (AHEAP*)Malloc(sizeof(AHEAP));
    header->max_size = size;
    header->size = size;
    header->intr_vector = (INTR*)Malloc(size*sizeof(INTR));
    
    return header;
}

void build_heap(AHEAP *header, HEAP_TYPE htype)
{
    //BUILD MAX HEAP
    if(htype == MAX)
    {
        for(int idx = (header->max_size/2)-1; idx >= 0; idx--)
            max_heapify(header, idx);
    }
    else
    { //MIN HEAP
        for(int idx = (header->max_size/2)-1; idx >= 0; idx--)
            min_heapify(header, idx);
    }
}

/*
 FOR USE WITH FIXED-ARRAY AND FOR HEAP SORT
 */
void max_heapify(AHEAP *heap, int parent_idx)
{
    int largest_idx, left_child_idx, right_child_idx;
    INTR temp;
    
    largest_idx = parent_idx;
    left_child_idx = 2*parent_idx + 1;
    right_child_idx = 2*parent_idx + 2;
    
    if(left_child_idx < heap->max_size && heap->intr_vector[left_child_idx].priority > heap->intr_vector[largest_idx].priority)
        largest_idx = left_child_idx;
    if(right_child_idx < heap->max_size && heap->intr_vector[right_child_idx].priority > heap->intr_vector[largest_idx].priority)
        largest_idx = right_child_idx;
    //if a child's value is greater than the parent
    if(largest_idx != parent_idx)
    {//exchange root value with largest child value
        temp = heap->intr_vector[parent_idx];
        heap->intr_vector[parent_idx] = heap->intr_vector[largest_idx];
        heap->intr_vector[largest_idx] = temp;
        //reheap...
        max_heapify(heap, largest_idx);
    }
    
}

void min_heapify(AHEAP *heap, int parent_idx)
{
    int smallest_idx, left_child_idx, right_child_idx;
    INTR temp;
    
    smallest_idx = parent_idx;
    left_child_idx = 2*parent_idx + 1;
    right_child_idx = 2*parent_idx + 2;
    
    if(left_child_idx < heap->max_size && heap->intr_vector[left_child_idx].priority < heap->intr_vector[smallest_idx].priority)
        smallest_idx = left_child_idx;
    if(right_child_idx < heap->max_size && heap->intr_vector[right_child_idx].priority < heap->intr_vector[smallest_idx].priority)
        smallest_idx = right_child_idx;
    //if a child's value is greater than the parent
    if(smallest_idx != parent_idx)
    {   //exchange root value with largest child value
        temp = heap->intr_vector[parent_idx];
        heap->intr_vector[parent_idx] = heap->intr_vector[smallest_idx];
        heap->intr_vector[smallest_idx] = temp;
        //reheap...
        max_heapify(heap, smallest_idx);
    }
    
}

INTR get_root(AHEAP *heap, HEAP_TYPE htype)
{
    INTR root, temp;
    root = heap->intr_vector[0];
    temp = heap->intr_vector[0];
    heap->intr_vector[0] = heap->intr_vector[heap->max_size-1];
    heap->max_size--;
    if(htype == MAX)
        reheap_down(heap, 0);//MAX HEAP
    else
        reheap_down_min(heap, 0);//MIN HEAP
    return root;
}

void reheap_down(AHEAP *heap, int parent)
{
    INTR hold, left_child_data, right_child_data;
    int left_child_idx, right_child_idx, largest_idx, last;
    
    last = heap->max_size - 1;
    left_child_idx = parent*2 + 1;
    if(left_child_idx <= last)// IF LEFT SUBTREE
    {
        left_child_data = heap->intr_vector[left_child_idx];
        right_child_idx = parent*2 + 2;
        if(right_child_idx <= last)//IF RIGHT SUBTREE
            right_child_data = heap->intr_vector[right_child_idx];
        else
            right_child_data.description = NULL;
        
        //DETERMINE WHICH CHILD HAS LARGEST VALUE
        if(!right_child_data.description || left_child_data.priority > right_child_data.priority)
            largest_idx = left_child_idx;
        else
            largest_idx = right_child_idx;
        //IF PARENT VALUE IS GREATER THAN THAT OF LARGEST CHILD'S VALUE
        if(heap->intr_vector[parent].priority < heap->intr_vector[largest_idx].priority)
        {
            hold = heap->intr_vector[parent];
            heap->intr_vector[parent] = heap->intr_vector[largest_idx];
            heap->intr_vector[largest_idx] = hold;
            reheap_down(heap, largest_idx);
        }
    }
}

void reheap_down_min(AHEAP *heap, int parent)
{
    INTR hold, left_child_data, right_child_data;
    int left_child_idx, right_child_idx, smallest_idx, last;
    last = heap->max_size - 1;
    left_child_idx = parent*2 + 1;
    if(left_child_idx <= last)// IF LEFT SUBTREE
    {
        left_child_data = heap->intr_vector[left_child_idx];
        right_child_idx = parent*2 + 2;
        if(right_child_idx <= last)//IF RIGHT SUBTREE
            right_child_data = heap->intr_vector[right_child_idx];
        else
            right_child_data.description = NULL;
        
        //DETERMINE WHICH CHILD HAS LARGEST VALUE
        if(!right_child_data.description || left_child_data.priority < right_child_data.priority)
            smallest_idx = left_child_idx;
        else
            smallest_idx = right_child_idx;
        //IF PARENT
        if(heap->intr_vector[parent].priority > heap->intr_vector[smallest_idx].priority)
        {
            hold = heap->intr_vector[parent];
            heap->intr_vector[parent] = heap->intr_vector[smallest_idx];
            heap->intr_vector[smallest_idx] = hold;
            reheap_down_min(heap, smallest_idx);
        }
    }
}

void heap_insert(AHEAP *heap, INTR data_in, HEAP_TYPE type)
{
    if(heap->max_size < heap->size)//if we can insert into fixed-length array
    {
        heap->max_size++;
        heap->intr_vector[heap->max_size-1] = data_in;
        if(type == MAX)
            reheap_up(heap, heap->max_size-1);
        else
            reheap_up_min(heap, heap->max_size-1);
    }
}

void reheap_up(AHEAP *heap, int child_idx)
{
    int parent_idx;
    INTR hold;
    
    if(child_idx)//not root
    {
        parent_idx = (child_idx-1)/2;
        if(heap->intr_vector[child_idx].priority >  heap->intr_vector[parent_idx].priority)
        {
            hold = heap->intr_vector[child_idx];
            heap->intr_vector[child_idx] = heap->intr_vector[parent_idx];
            heap->intr_vector[parent_idx] = hold;
            reheap_up(heap, parent_idx);
        }
    }
}

void reheap_up_min(AHEAP *heap, int child_idx)
{
    int parent_idx;
    INTR hold;
    
    if(child_idx)//not root
    {
        parent_idx = (child_idx-1)/2;
        if(heap->intr_vector[child_idx].priority <  heap->intr_vector[parent_idx].priority)
        {
            hold = heap->intr_vector[child_idx];
            heap->intr_vector[child_idx] = heap->intr_vector[parent_idx];
            heap->intr_vector[parent_idx] = hold;
            reheap_up_min(heap, parent_idx);
        }
    }
}


void print_tabs(int height)
{
    for(int levels = height; levels > 0; levels--)
        printf("\t");
}

void print_subtrees(int trees)
{
    while(trees > 0)
    {
        printf("/\\\t\t");
        trees--;
    }
    printf("\n");
    
}
void print_heap(AHEAP *heap)
{
    
    for(int i = 0; i < heap->max_size; i++)
        printf("%d:%s", heap->intr_vector[i].priority,heap->intr_vector[i].description);
    
    int subtree_idx, root_idx, level, height;
    printf("TREE FORM\n");
    height = floor(log2(heap->max_size))+1;//also the enumerates the different levels
    level = root_idx = 0;
    while(root_idx < heap->max_size)
    {
        print_tabs(height);
        subtree_idx = (int)pow(2, level);
        while(subtree_idx > 0 && root_idx < heap->max_size)
        {
            printf("%d\t\t", heap->intr_vector[root_idx].priority);
            subtree_idx--; root_idx++;
        }
        printf("\n");
        print_tabs(height);
        print_subtrees((int)pow(2,level));
        height--;
        level++;
    }
    printf("\n++++++++++++++++++++\n");
}

void destroy_heap(AHEAP *h)
{
    free(h->intr_vector);
    free(h);
}

/*****DYNAMIC VERSION*****/
/*****IF NO FRAGMENTATION.....****/
/*  assume: realloc is Malloc(new block size); memcpy(new block size, old block, old block size); free(old block);. */
DHEAP* build_dynamic_array_heap_adt(int size, int inc, HEAP_TYPE htype, IMPLEMENTATION_TYPE itype,
                                    int (*compare)(void *arg1, void *arg2), void (*process)(void *data))
{
    DHEAP *heap = (DHEAP*)Malloc(sizeof(DHEAP));
    heap->max_size = size;
    heap->realloc_amt = inc;//by how much we realloc, e.g., double, etc.
    heap->last = 0;
    heap->count = 0;
    heap->compare = compare;
    heap->process = process;
    heap->ary = (void**)calloc(heap->max_size, sizeof(void*));
    return heap;
}

bool dheap_insert(DHEAP *heap, void *data_in)
{
    if(heap->count == 0)
    {
        heap->count++;
        heap->last = 0;
        heap->ary[0] = data_in;
        return true;
    }
    if(heap->last == heap->max_size - 1)
    {
        //realloc extend or
        puts("realloc\n");
        heap->max_size = heap->max_size + heap->realloc_amt;
        heap->ary = (void**)realloc(heap->ary, heap->max_size);
    }
    heap->count++;
    heap->last++;
    heap->ary[heap->last] = data_in; // INSERT @ LEFT-MOST LEAF
    _dreheap_up(heap, heap->last);
    return true;
}

void _dreheap_up(DHEAP *heap, int child_idx)
{
    int parent_idx;
    void *hold;
    
    if(child_idx)
    {
        //compute parent and verify heap property
        parent_idx = (child_idx-1)/2;
        if(heap->compare(heap->ary[child_idx], heap->ary[parent_idx]) > 0)//for max heap
        {
            hold = heap->ary[child_idx];
            heap->ary[child_idx] = heap->ary[parent_idx];
            heap->ary[parent_idx] = hold;
            _dreheap_up(heap, parent_idx);
        }
    }
    
}

bool dheap_get_root(DHEAP *heap, void **data_out)
{
    if(heap->count == 0)
        return false;
    *data_out = heap->ary[0];
    heap->ary[0] = heap->ary[heap->last];
    heap->last--;
    heap->count--;
    _dreheap_down(heap, 0);
    return true;
}

void _dreheap_down(DHEAP *heap, int parent_idx)
{
    void *hold, *left_child_data, *right_child_data;
    int  last, left_child_idx, right_child_idx, largest_idx;
    
    last = heap->last;
    left_child_idx = 2*parent_idx + 1;
    if(left_child_idx)// IF LEFT SUBTREE
    {
        left_child_data = heap->ary[left_child_idx];
        right_child_idx = 2*parent_idx + 2;
        if(right_child_idx <= last)//IF RIGHT SUBTREE
            right_child_data = heap->ary[right_child_idx];
        else
            right_child_data = NULL;
        if(!right_child_data || (heap->compare(left_child_data, right_child_data) > 0))
            largest_idx = left_child_idx;
        else
            largest_idx = right_child_idx;
        //test for heap property
        if(heap->compare(heap->ary[parent_idx], heap->ary[largest_idx]) < 0)
        {
            hold = heap->ary[parent_idx];
            heap->ary[parent_idx] = heap->ary[largest_idx];
            heap->ary[largest_idx] = hold;
            _dreheap_down(heap, largest_idx);
        }
    }
}

//use a queue to print since breath first traversal....
void print_heap_ary(DHEAP *heap)
{
    for(int i = 0; i <  heap->count; i++)
        heap->process(heap->ary[i]);
}

void get_dheap_stats(DHEAP *heap)
{
    printf("max_size: %d\n", heap->max_size);
    printf("count: %d\n", heap->count);
    printf("leftmost leaf index: %d\n", heap->last);
}

void destroy_dynamic_heap_array(DHEAP *heap)
{
    free(heap->ary);
    free(heap);
}

/***********************************************************/
/******** FINE GRAINED MEMORY CONTROL... *******************/
/* must this be a tree structure or an array of structs.....*/
/*
 
 solution:
 each node has pointer to parent and left, right neightbors, and left, right children.
 
 have an array of levels. for each level Li, we compute n, and count. a level is full when count == n
 
 to determine where to insert, if level
 */

#define HEAP_SIZE_EXCEED printf("HEAP SIZE EXCEEDED!!!!!!!!");

LEVEL* set_levels_array(int n)
{
    LEVEL *ary;
    int levels;
    
    ary = (LEVEL*)calloc(n, sizeof(LEVEL));
    levels = floor(log2(n))+1;//from 0 to levels-1
    
    for(int i =0 ; i < levels; i++)
    {
        ary[i].n = (int)pow(2, i);
        ary[i].full = false;
        ary[i].count = 0;
    }
  return ary;
}

bool level_i_full(LEVEL *ary, int i)
{
    if(ary[i].count == ary[i].n)
    {
        ary[i].full = true;
        return true;
    }
    else
        return false;
}

PHEAP* create_pointer_based_heap(int size, int (*compare)(void *arg1, void *arg2), void (*process)(void *data), void (*swap)(void *arg1, void *arg2))
{
    PHEAP *heap = (PHEAP*)Malloc(sizeof(PHEAP));
    
    //FORCE SIZE TO A POWER OF 2
    heap->max_size = size;//(int)pow(2, ceil(log2(size))) - 1;
    heap->count = 0;
    heap->root = NULL;
    heap->last = NULL;
    heap->compare = compare;
    heap->process = process;
    heap->swap = swap;
    heap->ary = set_levels_array(size);
    return heap;
}

bool pointer_based_heap_insert(PHEAP *heap, void *data_in)
{
    HNODE *new_node;
    HNODE *left_i_1;
    HNODE *reheap_up;
    bool correct_loc;
    
    new_node = (HNODE*)Malloc(sizeof(HNODE));
    new_node->data = data_in;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->left_neighbor = NULL;
    //new_node->right_neighbor = NULL;//DON'T NEED TO DO THIS
    
    if(heap->count == 0)//L0
    {
        heap->root = new_node;
        //heap->root->left_neighbor = heap->root->right_neighbor = NULL;
        new_node->parent = NULL;
        heap->ary[0].count = 1;
        heap->ary[0].full = true;
        heap->current_level = 1;
    }
    else //1) FIND LEFT MOST LEFT TO INSERT NEW NODE.
    {
        //TEST CURRENT LEVEL FULL
        if(level_i_full(heap->ary, heap->current_level))
            heap->current_level++;//go to next level
        
        //TRAVERSE ALL THE WAY DOWN TO LEFT DOWN TO LEVEL i - 1
        left_i_1 = heap->root;
        for(int level = 0; level < heap->current_level - 1; level++)
        {
            left_i_1 = left_i_1->left;
        }
        //TEST THE CHILDREN
        //MOVE HORIZONTALLY ACROSS NEIGHBORS UNTIL YOU FIND ONE WITH A NIL CHILD.
        while((left_i_1->left && left_i_1->right) && left_i_1 != NULL)
        {
            left_i_1 = left_i_1->right_neighbor;
        }
        //FOUND A NEIGHBOR WITH NIL CHILD
        if(!left_i_1->left)//LEFT CHILD
        {
            heap->ary[heap->current_level].count++;
            left_i_1->left = new_node;
            new_node->parent = left_i_1;
            if(heap->ary[heap->current_level].count == 1)//EDGE LEFT CHILD
                new_node->left_neighbor = NULL;
            else//INTERNAL LEFT CHILD
                new_node->left_neighbor = left_i_1->left_neighbor->right;//NIL
            new_node->right_neighbor = left_i_1->right;
        }
        else //if(!left_i_1->right)//RIGHT CHILD
        {
            heap->ary[heap->current_level].count++;
            left_i_1->right = new_node; //*
            new_node->parent = left_i_1;
            new_node->left_neighbor = left_i_1->left;
            //should not have to do this --
            new_node->left_neighbor->right_neighbor = new_node;//JUST UPDATE HERE
            //pointer for left neighbor should be updated as parent's right pointer changes.
            if(heap->ary[heap->current_level].count == heap->ary[heap->current_level].n)//EDGE RIGHT CHILD
                new_node->right_neighbor = NULL;
            else//INTERNAL RIGHT CHILD
                new_node->right_neighbor = left_i_1->right_neighbor->left;
         }
        // 2) VERIFY HEAP PROPERTY CLIMBING UP THE TREE WITH PARENT POINTERS
        reheap_up = new_node;
        correct_loc = false;
        while(!correct_loc && reheap_up->parent)
        {
            //FOR MAX HEAP, IF CHILD DATA > PARENT DATA SWAP
            if(heap->compare(reheap_up->data, reheap_up->parent->data) > 0)
            {
                heap->swap(reheap_up->data, reheap_up->parent->data);
                reheap_up = reheap_up->parent;
            }
            else
                correct_loc = true;
        }//new data at correct location.
    }
    heap->last = new_node;
    heap->count++;

    return true;
}

void pointer_based_heap_get_root(PHEAP *heap)
{
    if(heap->count != 0)
    {
        heap->process(heap->root->data);
        pointer_based_heap_delete(heap);
    }
}

bool pointer_based_heap_delete(PHEAP *heap)
{
    HNODE *last;
    HNODE *reheap_down;
    HNODE *largest_child;
    bool correct_loc;
    int working_level;
    
    //1) FIND LEFT MOST LEFT LEAF
    //ENSURE CURRENT LEVEL IS CORRECT
    if(level_i_full(heap->ary, heap->current_level))
        heap->current_level++;//go to next level
    
    //WE NEED WORK ON FIRST NON-EMPTY LEVEL
    if(heap->ary[heap->current_level].count > 0)
        working_level = heap->current_level;
    else
        working_level = heap->current_level - 1;
    
    //printf("cl: %d\n", heap->current_level);
    //2) USE BREATH FIRST SEARCH TO FIND LAST NODE IN THE TREE
    //actually, no.
    last = heap->last;
    
    //2) SWAP DATA WITH ROOT LAST NODE
    heap->swap(heap->root->data, last->data);
    
    //4) DELETE LAST NODE AND RESET ANY POINTERS TO NIL WHICH SHOULD BE
    if(heap->count > 1) //IF NOT ONLY NODE
    {
        
        //RESET PARENT POINTER TO CHILD
        //IS LAST A LEFT OR RIGHT CHILD?
        if(!last->parent->right) //LEFT CHILD
        {
            if(heap->ary[working_level].count == 1)//edge node -- not enough to determine left edge node
            {
                heap->last = last->parent->right_neighbor;//update the last poiter
                while(heap->last->right_neighbor)
                {
                    heap->last = heap->last->right_neighbor; //****
                }
                last->parent->left = NULL;//set parent->last to NIL
            }
            else//internal left node --
            {
                heap->last = last->left_neighbor;
                last->left_neighbor->right_neighbor = NULL;//left neighbor's right neighbor pointer
                last->parent->left = NULL;//reset parent left pointer
            }
        }
        else//RIGHT CHILD
        {
            heap->last = last->left_neighbor;
            //same pointers to reset for an edge or internal node
            last->left_neighbor->right_neighbor = NULL;//left neighbor's right neighbor pointer
            last->parent->right = NULL;//reset parent left pointer
            
        }
        heap->ary[working_level].count--;
        free(last);
        //5) FROM THE ROOT, REHEAP DOWN.... SWAPING DATA AS NEEDED TO KEEP HEAP PROPERTY.
        //complete
        //this can be done recursibely as well....
        if(heap->count >= 2)//delted othe only child
        {
            reheap_down = heap->root;
            correct_loc = false;
            while(!correct_loc)
            {
                //IF THERE ARE CHILDREN, DETERMINE ONE WITH LARGEST KEY
                if(reheap_down->left)
                {
                    if(reheap_down->right)
                    {
                        if(heap->compare(reheap_down->right->data, reheap_down->left->data) > 0)
                            largest_child = reheap_down->right;
                    }
                    largest_child = reheap_down->left;
                    
                    if(heap->compare(reheap_down->data, largest_child->data) < 0)
                    {
                        heap->swap(reheap_down->data, largest_child->data);
                        //go down the tree=: recursive call... with left or right as root.
                        if(largest_child == reheap_down->left)
                            reheap_down = reheap_down->left;
                        else
                            reheap_down = reheap_down->right;
                    }
                    else
                        correct_loc = true;
                }
                else
                    correct_loc = true;
              }
        }
        
    }
    else //nothing to reset, delete the root.
    {
        heap->ary[working_level].count--;
        heap->last = NULL;
        free(last);
        heap->root = NULL;
    }
    heap->count--;
    return true;
}

void pointer_based_heap_breath_first_traversal(PHEAP *heap)
{
    QUEUE *q;
    HNODE *cur;

    q = create_queue_adt(NULL);
    enqueue_queue_adt(q, heap->root);
    while(!is_queue_empty_queue_adt(q))
    {
        cur =  (HNODE*)deque_queue_adt(q);
        heap->process(cur->data);
        if(cur->left)
            enqueue_queue_adt(q, cur->left);
        if(cur->right)
            enqueue_queue_adt(q, cur->right);
    }
}

void destroy_pointer_based_heap(PHEAP *heap)
{
   free(heap);
}

/******** HEAP ADT as we have seen the signature of ADTs...
we use for graphs
*******/
HEAP_ADT* heap_adt(int size, int (*compare)(void *arg1, void *arg2))
{
    HEAP_ADT *heap = (HEAP_ADT*)Malloc(sizeof(HEAP_ADT));
    heap->size = size;
    heap->compare = compare;
    heap->count = 0;
    heap->ary = (void**)calloc(size, sizeof(void*));
    for(int i =0; i < size; i++)
        heap->ary[i] = (void*)calloc(1,sizeof(void));
    return heap;
}

void build_min_heap(HEAP_ADT* heap)
{
    for(int idx = (heap->size/2)-1; idx >= 0; idx--)
        min_heapify_adt(heap, idx);
}

void min_heapify_adt(HEAP_ADT *heap, int parent_idx)
{
    int smallest_idx, left_child_idx, right_child_idx;
    void *temp;
    
    smallest_idx = parent_idx;
    left_child_idx = 2*parent_idx + 1;
    right_child_idx = 2*parent_idx + 2;
    
    if(left_child_idx < heap->size && heap->compare(heap->ary[left_child_idx], heap->ary[smallest_idx]) < 0)
        smallest_idx = left_child_idx;
    if(right_child_idx < heap->size && heap->compare(heap->ary[right_child_idx], heap->ary[smallest_idx]) < 0)
        smallest_idx = right_child_idx;
    //if a child's value is greater than the parent
    if(smallest_idx != parent_idx)
    {   //exchange root value with largest child value
        temp = heap->ary[parent_idx];
        heap->ary[parent_idx] = heap->ary[smallest_idx];
        heap->ary[smallest_idx] = temp;
        //reheap...
        min_heapify_adt(heap, smallest_idx);
    }
}

void heap_insert_adt(HEAP_ADT *heap, void *data_in)
{
    if(heap->count < heap->size)//if we can insert into fixed-length array
    {
        heap->count++;
        heap->ary[heap->size-1] = data_in;
        reheap_up_min_adt(heap, heap->size-1);
    }
}

void reheap_up_min_adt(HEAP_ADT *heap, int child_idx)
{
    int parent_idx;
    void *hold;
    
    if(child_idx)//not root
    {
        parent_idx = (child_idx-1)/2;
        //  if(heap->ary[child_idx].key <  heap->ary[parent_idx].key)
        if(heap->compare(heap->ary[child_idx], heap->ary[parent_idx]) < 0)
        {
            hold = heap->ary[child_idx];
            heap->ary[child_idx] = heap->ary[parent_idx];
            heap->ary[parent_idx] = hold;
            reheap_up_min_adt(heap, parent_idx);
        }
    }
}

void* get_min(HEAP_ADT *heap)
{
    void *root = heap->ary[0];
    heap->ary[0] = heap->ary[heap->size-1];
    heap->size--;
    reheap_down_min_adt(heap, 0);//MIN HEAP
    return root;
}

void reheap_down_min_adt(HEAP_ADT *heap, int parent)
{
    void *hold, *left_child_data, *right_child_data;
    int left_child_idx, right_child_idx, smallest_idx, last;
    
    last = heap->size - 1;
    left_child_idx = parent*2 + 1;
    
    if(left_child_idx <= last)// IF LEFT SUBTREE
    {
        left_child_data = heap->ary[left_child_idx];
        right_child_idx = parent*2 + 2;
        if(right_child_idx <= last)//IF RIGHT SUBTREE
            right_child_data = heap->ary[right_child_idx];
        else
            right_child_data = NULL;
        
        //DETERMINE WHICH CHILD HAS LARGEST VALUE
        if(!right_child_data || (heap->compare(left_child_data, right_child_data) < 0))
            smallest_idx = left_child_idx;
        else
            smallest_idx = right_child_idx;
        //IF PARENT
        if(heap->compare(heap->ary[parent], heap->ary[smallest_idx]) > 0)
        {
            hold = heap->ary[parent];
            heap->ary[parent] = heap->ary[smallest_idx];
            heap->ary[smallest_idx] = hold;
            reheap_down_min_adt(heap, smallest_idx);
        }
    }
}

void destroy_heap_adt(HEAP_ADT *heap)
{
    free(heap->ary);
    free(heap);
}

#endif /* heap_h */

