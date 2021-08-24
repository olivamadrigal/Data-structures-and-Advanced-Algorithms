#ifndef graph_h
#define graph_h
/* Both implementations: adjacency matrix and linked-list. Later allows to handle cases, such as multiple links to and from a source
and is widely used in networking. Former has its use cases.*/
#include "limits.h"
#include "wrappers.h"
#include "queue.h"
#include "stack.h"
#include "heap.h"

/***************************************************************/
typedef enum {MATRIX=1,ADJACENCY_LIST=2} GRAPH_IMPLEMENTATION_TYPE;
typedef enum {DIRECTED=1,UNDIRECTED=2} GRAPH_TYPE;
typedef enum {NON_WEIGHTED=0,IS_WEIGHTED=1} WEIGHTED;

typedef struct vertex VERTEX;


typedef struct edge_element
{
    struct vertex *src;
    struct vertex *dst;
    struct edge_element *next;
    int weight;
    
}EDGE_ELEMENT;

typedef struct arc_element
{
    struct vertex *src;
    struct vertex *dst;
    struct arc_element *next;
    int weight;
}ARC_ELEMENT;


typedef struct arc
{
    struct vertex *dest;
    struct arc *next;
    int weight;
    bool in_mst;
}ARC;

typedef struct edge
{
    struct vertex *dest;
    struct edge *next;
    int weight;
    
}EDGE;

typedef struct vertex
{
    void *data;
    //*********adj list only*********/
    struct vertex *next;
    struct arc *adj_list;
    struct edge *edge_list;
    //*********adj list only*******?
    int in_degree;
    int out_degree;
    int degree;
    int processed;
    short in_msp;
    //MST
    int key;
    int d;
    struct vertex *pi;
    int kids;
}VERTEX;

typedef struct
{
    int count;//for vertices
    int link_count;//for edges;
    int arc_count;//for digraphs
    //APPLICATION-SPECIFIC POINTERS TO KEY COMPARE AND PROCESS FUNCTIONS
    int (*compare)(void *arg1, void *arg2);
    void (*process)(void *data);
    //********adjacency list only*****/
    struct vertex *source;
    struct edge_element *E;//SET OF EDGES
    struct arc_element *A;//SET OF EDGES
    /***************matrix only********/
    VERTEX **ary;
    int **matrix;
    int size;
    /*********************************/
    WEIGHTED w;
    GRAPH_TYPE d;
    GRAPH_IMPLEMENTATION_TYPE i_type;
}GRAPH;

GRAPH *create_graph(GRAPH_IMPLEMENTATION_TYPE i_type,
                    GRAPH_TYPE g_type,
                    WEIGHTED w_type, int size,
                    int (*compare)(void *arg1, void *arg2),
                    void (*process)(void *data));
//ADT API
bool insert_to_graph(GRAPH *g, void *data_in);
int delete_from_graph(GRAPH *g, void *data_in);
bool search_graph(GRAPH *g, void *data_in);
void *retrieve_graph(GRAPH *g, void *data_in);
int  add_arc_edge_to_graph(GRAPH *g, void *from, void *to, int weight);
int  del_arc_edge_from_graph(GRAPH *g, void *from, void *to);
void traverse_graph(GRAPH *g);
void depth_first_graph_traversal(GRAPH *g);
void breadth_first_graph_traversal(GRAPH *g);
void delete_graph(GRAPH *g);
bool add_to_arc_edge_set(GRAPH *g);

//MATRIX API
bool insert_g_matrix(GRAPH *g, void *data_in);
bool delete_g_matrix(GRAPH *g, void *data_in);
bool search_g_matrix(GRAPH *g, void *data_in);
void search_g_matrix_loc(GRAPH *g, void *target, int *loc, bool *success);
void *retrieve_g_matrix(GRAPH *g, void *data_in);
bool add_arc_edge_g_matrix(GRAPH *g, void *from, void *to, int weight);
bool del_arc_edge_g_matrix(GRAPH *g, void *from, void *to);
void traverse_matrix(GRAPH *g);
void depth_first_matrix_graph_traversal(GRAPH *g);
void breadth_first_matrix_graph_traversal(GRAPH *g);
void delete_matrix_graph(GRAPH *g);
void get_set_of_vertices_list_undirected_graph(GRAPH *g);
bool add_to_arc_edge_matrix_set(GRAPH *g);

//LIST API
bool insert_g_list(GRAPH *g, void *data_in);
int delete_g_list(GRAPH *g, void *target);
bool search_g_list(GRAPH *g, void *target);
void *retrieve_g_list(GRAPH *g, void *data_in);
int add_arc_g_list(GRAPH *g, void *from, void *to, int weight);
int add_edge_g_list(GRAPH *g, void *from, void *to, int weight);
int del_arc_g_list(GRAPH *g, void *from, void *to);
int del_edge_g_list(GRAPH *g, void *from, void *to);
void traverse_undirected_list(GRAPH *g);
void traverse_directed_list(GRAPH *g);
void depth_first_list_digraph_traversal(GRAPH *g);
void breadth_first_list_digraph_traversal(GRAPH *g);
void depth_first_list_graph_traversal(GRAPH *g);
void breadth_first_list_graph_traversal(GRAPH *g);
void delete_list_directed_graph(GRAPH *g);
void delete_list_undirected_graph(GRAPH *g);
bool add_to_edge_list_set(GRAPH *g);
bool add_to_arc_list_set(GRAPH *g);


/************** GRAPH ADT API ******************/
GRAPH *create_graph(GRAPH_IMPLEMENTATION_TYPE i_type, GRAPH_TYPE g_type, WEIGHTED w_type, int size,
                    int (*compare)(void *arg1, void *arg2), void (*process)(void *data))
{

    GRAPH *g = (GRAPH*)malloc(sizeof(GRAPH));
    g->count = 0;
    g->arc_count = 0;
    g->link_count = 0;
    if(i_type == MATRIX)
    {
        g->size = size;
        g->ary = (VERTEX**)calloc(size, sizeof(VERTEX*));
        g->matrix = (int**)calloc(size, sizeof(int*));
        for(int i = 0; i < size; i++)
            g->matrix[i] = (int*)calloc(size, sizeof(int));
    }
    else
    {
        g->source = NULL;
    }
    g->compare = compare;
    g->process = process;
    g->w = w_type;
    g->d = g_type;
    g->i_type = i_type;
    
    return g;
}

bool insert_to_graph(GRAPH *g, void *data_in)
{
    bool success = false;
    if(g->i_type == MATRIX)
        success = insert_g_matrix(g, data_in);
    else
        success = insert_g_list(g, data_in);
    return success;
}

int delete_from_graph(GRAPH *g, void *data_out)
{
  int res;
  if(g->i_type == MATRIX)
    res = (int) delete_g_matrix(g, data_out);
  else
    res = delete_g_list(g, data_out);
  return res;
}

bool search_graph(GRAPH *g, void *data_out)
{
  bool found = false;
  if(g->i_type == MATRIX)
    found = search_g_matrix(g, data_out);
  else
    found = search_g_list(g, data_out);
  return found;
  
}

void* retrieve_graph(GRAPH *g, void *data_out)
{
  void *data = NULL;
  
  if(g->i_type == MATRIX)
    data = retrieve_g_matrix(g, data_out);
  else
    data = retrieve_g_list(g, data_out);
    
  return data;
}

int add_arc_edge_to_graph(GRAPH *g, void *from, void *to, int weight)
{
  int res;
  if(g->i_type == MATRIX)
    res = (int) add_arc_edge_g_matrix(g, from, to, weight);
  else
  {
    if(g->d == DIRECTED)
        res = add_arc_g_list(g, from, to, weight);
    else
        res = add_edge_g_list(g, from, to, weight);
    
  }
  return res;
}

int del_arc_edge_from_graph(GRAPH *g, void *from, void *to)
{
  int res;
  if(g->i_type == MATRIX)
     res = del_arc_edge_g_matrix(g, from, to);
  else
  {
    if(g->d == DIRECTED)
        res = del_arc_g_list(g, from, to);
    else
        res = del_edge_g_list(g, from, to);
   }
  return res;
}
void traverse_graph(GRAPH *g)
{
  if(g->i_type == MATRIX)
    traverse_matrix(g);
  else
    {
        if(g->d == DIRECTED)
            traverse_directed_list(g);
        else
            traverse_undirected_list(g);
    }
}

void depth_first_graph_traversal(GRAPH *g)
{
  if(g->i_type == MATRIX)
    depth_first_matrix_graph_traversal(g);
  else
  {
    if(g->d == DIRECTED)
        depth_first_list_digraph_traversal(g);
    else
         depth_first_list_graph_traversal(g);
  }

}

void breadth_first_graph_traversal(GRAPH *g)
{
    if(g->i_type == MATRIX)
        breadth_first_matrix_graph_traversal(g);
    else
    {
        if(g->d == DIRECTED)
            breadth_first_list_digraph_traversal(g);
        else
            breadth_first_list_graph_traversal(g);
    }
}

void delete_graph(GRAPH *g)
{
    if(g->i_type == MATRIX)
        delete_matrix_graph(g);
    else
    {
        if(g->d == DIRECTED)
            delete_list_directed_graph(g);
        else
            delete_list_undirected_graph(g);
    }
}

/************** MATRIX API ******************/
bool insert_g_matrix(GRAPH *g, void *data_in)
{
    bool success = false;
    
    VERTEX *new_vertex = (VERTEX*)malloc(sizeof(VERTEX));
    
    if(new_vertex)
    {
        new_vertex->data = data_in;
        new_vertex->in_degree = 0;
        new_vertex->out_degree = 0;
        new_vertex->processed = false;
        new_vertex->in_msp = false;
        success = true;
    }
    else
    {
        MALLOC_ERROR;
        return success;
    }
    g->ary[g->count] = new_vertex;
    g->count++;
    return success;
}

bool delete_g_matrix(GRAPH *g, void *data_out)
{
    bool success = false;

    if(g->count == 0)
        return success;
    else
    {
        for(int i = 0; i < g->size; i++)
        {
            if(g->ary[i])
            {
                if(g->compare(g->ary[i]->data, data_out) == 0)
                {
                    g->ary[i]->data = NULL;
                    success= true;
                    g->count--;
                    i = g->size;
                }
            }//if_if
            
        }//for
    }//if_else
    
    return success;
}

bool search_g_matrix(GRAPH *g, void *target)
{
    bool success = false;

    if(g->count == 0)
        return success;
    else
    {
        for(int i = 0; i < g->size; i++)
        {
            if(g->ary[i])
            {
                if(g->compare(g->ary[i]->data, target) == 0)
                {
                    success= true;
                    i = g->size;
                }
            }//if_if
            
        }//for
    }//if_else
    return success;
}

void search_g_matrix_loc(GRAPH *g, void *target, int *loc, bool *success)
{
    if(g->count == 0)
    {
        *success = false;
        *loc = -1;
        return;
     }
    else
    {
        for(int i = 0; i < g->size; i++)
        {
            if(g->ary[i])
            {
                if(g->compare(g->ary[i]->data, target) == 0)
                {
                    *success = true;
                    *loc = i;
                    i = g->size;
                }
            }//if_if
            
        }//for
    }//if_else
    
}

void *retrieve_g_matrix(GRAPH *g, void *data_out)
{
    void *data = NULL;
    
    if(g->count == 0)
        return data;
    else
    {
        for(int i = 0; i < g->size; i++)
        {
            if(g->ary[i])
            {
                if(g->compare(g->ary[i]->data, data_out) == 0)
                {
                    data = g->ary[i]->data;
                    i = g->size;
                }
            }//if_if
            
        }//for
    }//if_else
    return data;
}

bool add_arc_edge_g_matrix(GRAPH *g, void *from, void *to, int weight)
{
    bool success;
    int  loc;
    int  src, dst, value;
    
    //search for from vertex
    search_g_matrix_loc(g, from, &loc, &success);
    if(!success)
        return success;
    src = loc;
    //search for to vertex
    search_g_matrix_loc(g, to, &loc, &success);
    if(!success)
        return success;
    dst = loc;

    //update degrees
    if(g->d == DIRECTED)
    {
        g->ary[src]->out_degree++;
        g->ary[src]->degree = g->ary[src]->out_degree + g->ary[src]->in_degree;
        g->ary[dst]->in_degree++;
        g->ary[dst]->degree = g->ary[dst]->out_degree + g->ary[dst]->in_degree;
        g->arc_count++;
    }
    else
        g->link_count++;
    
    //found both source and destination which define the edge
    if(g->w == IS_WEIGHTED)
         value = weight;
    else
         value = 1;
    
    //we use the row as defined src ---> dst.
    g->matrix[src][dst] = value; // ARC
    if(g->d == UNDIRECTED)       // BECOMES EDGE
        g->matrix[dst][src] = value;
    
    return success;
}

bool del_arc_edge_g_matrix(GRAPH *g, void *from, void *to)
{
    bool success;
    int  loc;
    int  src, dst, value;
    
    //search for from vertex
    search_g_matrix_loc(g, from, &loc, &success);
    if(!success)
        return success;
    src = loc;
    //search for to vertex
    search_g_matrix_loc(g, to, &loc, &success);
    if(!success)
        return success;
    dst = loc;

    //found both source and destination which define the edge
    if(g->w == IS_WEIGHTED)
         value = 0;//for now
    else
         value = 0;
    
    //we use the row as defined src ---> dst.
    g->matrix[src][dst] = value; // ARC
    if(g->d == UNDIRECTED)       // BECOMES EDGE
        g->matrix[dst][src] = value;
    
    //update degrees
    if(g->d == DIRECTED)
    {
        g->ary[src]->out_degree--;
        g->ary[src]->degree = g->ary[src]->out_degree + g->ary[src]->in_degree;
        g->ary[dst]->in_degree--;
        g->ary[dst]->degree = g->ary[dst]->out_degree + g->ary[dst]->in_degree;
        g->arc_count--;
    }
    else
        g->link_count--;
    
    return success;

}

void traverse_matrix(GRAPH *g)
{
    if(g->count == 0)
        return;
    //first print vector and matrix
    printf("adjacency matrix: \n");
    printf("  ");
    for(int i = 0; i < g->count; i++)
        g->process(g->ary[i]->data);
    printf("\n");
    for(int i = 0; i < g->count; i++)
    {
        
        g->process(g->ary[i]->data);
        for(int j = 0; j < g->count; j++)
            printf("%d ",g->matrix[i][j]);
        printf("\n");
    }
}

void delete_matrix_graph(GRAPH *g)
{

    free(g->ary);
    for(int i = 0; i < g->size; i++)
        free(g->matrix[i]);
    free(g->matrix);
    free(g);
}


/*
PROCESS ALL DESCENDANTS OF A NODE BEFORE MOVING ON TO A SIBLING
WE MOVE VERTICALLY.... easier to see from a tree and then draw
a graph "level"-wise.
*/
void depth_first_matrix_graph_traversal(GRAPH *g)
{
    VERTEX *v;
    STACK *s;
    int loc;
    bool success;
    
    if(g->count == 0)
        return;
    
    s = create_stack_adt(g->process);
    
    //SET PROCESSED FLAGS
    for(int i = 0; i < g->size; i++)
        g->ary[i]->processed = 0;
    
    //PUSH SOURCE TO STACK
    push_stack_adt(s,  g->ary[0]);
    g->ary[0]->processed = 1;

    //PROCESS GRAPH IN DEPTH 1ST ORDER
    while(!is_empty_stack_adt(s))
    {
        v = pop_stack_adt(s);
        g->process(v->data);
        printf("\n");
        v->processed = 2;//BEEN PROCESSED
        
        //locate vertex v in the adjacency array
        search_g_matrix_loc(g, v->data, &loc, &success);
        if(success)
        {
            //process its adjacency list
            for(int j=0; j < g->size; j++)
            {
                if(g->matrix[loc][j] != 0)//IS NEIGHBOR?
                {
                    if(g->ary[j]->processed < 1) //BEEN PUSHED?
                    {
                        printf("PUSHING %d\n", j);
                        push_stack_adt(s, g->ary[j]);
                        g->ary[j]->processed = 1;
                    }
                }
            }

        }

    }
    destroy_stack_adt(s);
}

/*
PROCESS ALL SIBLING NODES BEFORE DESCENDANTS
WE MOVE HORIZONTALLY.... in a tree, we process all nodes per level
as we go down the tree, same with a graph.. easier to see if we
draw the graph "level"-wise.
*/
void breadth_first_matrix_graph_traversal(GRAPH *g)
{
    QUEUE *q;
    VERTEX *v;
    bool success;
    int loc;
    
    if(g->count == 0)
        return;
    
    for(int i = 0; i < g->size; i++)
        g->ary[i]->processed = 0;
    
    q = create_queue_adt(g->process);
    enqueue_queue_adt(q, g->ary[0]);
    g->ary[0]->processed = 1;
    
    while(!is_queue_empty_queue_adt(q))
    {
        v =  deque_queue_adt(q);
        g->process(v->data);
        v->processed = 2;
        search_g_matrix_loc(g, v->data, &loc, &success);
        if(success)//IF FOUND NODE IN GRAPH
        {
            //PROCESS ADJACENCY LIST
            for(int j=0; j < g->count; j++)
            {
                if(g->matrix[loc][j] != 0)
                {
                    if(g->ary[j]->processed < 1)
                    {
                        enqueue_queue_adt(q, g->ary[j]);
                        g->ary[j]->processed = 1;
                    }
                }
            }//process adj list
        }//if found
    }//while queue not empty
    
}

/************** LIST ADT API *****************/
/* implementation specifics really depends on the application,
such as updating the source and inserting as
an "ordereded linked list" */
bool insert_g_list(GRAPH *g, void *data_in)
{
    VERTEX *v_new, *loc, *pre;
    bool success = false;
    //allocate new vertex
    v_new = (VERTEX*)malloc(sizeof(VERTEX));
    if(v_new)
    {
        v_new->data = data_in;
        v_new->next  = NULL;
        v_new->adj_list = NULL;
        v_new->edge_list = NULL;
        v_new->in_degree = 0;
        v_new->out_degree = 0;
        g->count++;
    }
    //allocate vertex at correct location
    loc = g->source;
    if(!loc)
    {
        g->source = v_new;
        success = true;
    }
    else
    {
        pre = NULL;
        while(loc && (g->compare(data_in, loc->data) > 0))
        {
            pre = loc;
            loc = loc->next;
        }
        if(!pre)//insert at front
        {
            v_new->next = g->source;
            g->source = v_new;
        }
        else//middle or end
        {
            v_new->next = pre->next;
            pre->next = v_new;
        }
        success = true;
    }
    return success;
}

int delete_g_list(GRAPH *g, void *target)
{
    VERTEX *pre, *loc;
    bool success;
    
    success = false;
    if(g->count == 0)
        return success;
    
    //locate target
    pre = NULL;
    loc = g->source;
    while(loc && (g->compare(target, loc->data) > 0))
    {
        pre = loc;
        loc = loc->next;
    }
    //use short circuit if key not found
    if(!loc || g->compare(target, loc->data) != 0)
        return -1;
    //if degree != 0, cannot remove such a vertext
    if(loc->in_degree > 0 || loc->out_degree > 0)
        return -2;
    if(!pre)//delete @ front
        g->source = g->source->next;
    else
        pre->next = pre->next->next;
    g->count--;
    free(loc);
    return +1;
}

bool search_g_list(GRAPH *g, void *target)
{
    VERTEX *pre, *loc;
    
    pre = NULL;
    loc = g->source;
    while(loc && (g->compare(target, loc->data) > 0))
    {
        pre = loc;
        loc = loc->next;
    }
    if(!loc || (g->compare(target, loc->data) != 0))
        return true;
    else
        return false;
}

void *retrieve_g_list(GRAPH *g, void *target)
{
    VERTEX *pre, *loc;
    
    pre = NULL;
    loc = g->source;
    while(loc && (g->compare(target, loc->data) > 0))
    {
        pre = loc;
        loc = loc->next;
    }
    if(!loc || (g->compare(target, loc->data) != 0))
        return NULL;
    else
        return loc;

}

int add_arc_g_list(GRAPH *g, void *from, void *to, int weight)
{
    ARC *a_new, *pre, *loc;
    VERTEX *src, *dst;
    
    //locate the from vertex
    src = g->source;
    while(src && (g->compare(from, src->data) > 0))
        src = src->next;
    if(!src || g->compare(from, src->data) != 0)
        return -1;
    //locate the to vertext
    dst = g->source;
    while(dst && (g->compare(to, dst->data) > 0))
        dst = dst->next;
    if(!dst || g->compare(to, dst->data) != 0)
        return -2;
    //create arc and insert arc
    a_new = (ARC*)malloc(sizeof(ARC));
    if(!a_new)
    {
        MALLOC_ERROR;
        return -3;
    }
    a_new->dest = dst;
    //insert arc in src adjancency list
    loc = src->adj_list;
    if(!loc)
    {
        src->adj_list = a_new;
        a_new->next = NULL;
    }
    else
    {
        pre = NULL;
        while(loc && (g->compare(to, loc->dest->data) > 0))
        {
            pre = loc;
            loc = loc->next;
        }
        if(!pre) //insert @ front
            src->adj_list = a_new;
        else//insert @ middle or front
            pre->next = a_new;
        a_new->next = loc;
    }
    //update degrees
    if(g->d == DIRECTED)
    {
        src->out_degree++;
        dst->in_degree++;
        src->degree = src->in_degree + src->out_degree;
        dst->degree = dst->in_degree + dst->out_degree;
    }
    if(g->w == IS_WEIGHTED)
        a_new->weight = weight;
    
    return 1;
}

int add_edge_g_list(GRAPH *g, void *from, void *to, int weight)
{
    EDGE *e_new, *pre, *loc;
    VERTEX *src, *dst;
    
    //locate the from vertex
    src = g->source;
    while(src && (g->compare(from, src->data) > 0))
        src = src->next;
    if(!src || g->compare(from, src->data) != 0)
        return -1;
    //locate the to vertext
    dst = g->source;
    while(dst && (g->compare(to, dst->data) > 0))
        dst = dst->next;
    if(!dst || (g->compare(to, dst->data) != 0))
        return -2;
    
    //BEASE DIRECTED, WE ADD EDGE TO BOTH....
    //sourc --- dest and vice versa
    e_new = (EDGE*)malloc(sizeof(EDGE));
    if(!e_new)
    {
        MALLOC_ERROR;
        return -3;
    }
    e_new->dest = dst;
    //insert arc in src adjancency list
    loc = src->edge_list;
    if(!loc)
    {
        src->edge_list = e_new;
        e_new->next = NULL;
    }
    else
    {
        pre = NULL;
        while(loc && (g->compare(to, loc->dest->data) > 0))
        {
            pre = loc;
            loc = loc->next;
        }
        if(!pre) //insert @ front
            src->edge_list = e_new;
        else//insert @ middle or front
            pre->next = e_new;
        e_new->next = loc;
    }
    if(g->w == IS_WEIGHTED)
        e_new->weight = weight;
    //FROM THE OTHER DIRECTION
    e_new = (EDGE*)malloc(sizeof(EDGE));
    if(!e_new)
    {
        MALLOC_ERROR;
        return -4;
    }
    e_new->dest = src;
    //insert arc in src adjancency list
    loc = dst->edge_list;
    if(!loc)
    {
        dst->edge_list = e_new;
        e_new->next = NULL;
    }
    else
    {
        pre = NULL;
        while(loc && (g->compare(from, loc->dest->data) > 0))
        {
            pre = loc;
            loc = loc->next;
        }
        if(!pre) //insert @ front
            dst->edge_list = e_new;
        else//insert @ middle or front
            pre->next = e_new;
        e_new->next = loc;
    }
    if(g->w == IS_WEIGHTED)
        e_new->weight = weight;
    return 1;
}

int del_arc_g_list(GRAPH *g, void *from, void *to)
{
    ARC  *pre, *loc;
    VERTEX *src, *dst;
    
    //VERIFY ARC EXISTS
    //locate source
    src= g->source;
    while(src && (g->compare(src->data, from) > 0))
        src = src->next;
    if(!src || (g->compare(src->data, from)!=0))
        return -1;
    //locate dst
    dst = g->source;
    while(dst && (g->compare(dst->data, to) > 0))
        dst = dst->next;
    if(!dst || (g->compare(dst->data, to) != 0))
        return -2;
    //locate arc
    loc = src->adj_list;
    if(!loc)
        return -3;
    pre = NULL;
    while(loc && (g->compare(loc->dest->data, to) > 0))
    {
        pre = loc;
        loc = loc->next;
    }
    if(!loc || (g->compare(loc->dest->data, to)!=0))
        return -4;
    if(!pre)//del @ front
        src->adj_list = src->adj_list->next;
    else
        pre->next = pre->next->next;
    free(loc);
    return 1;
}

int del_edge_g_list(GRAPH *g, void *from, void *to)
{
    EDGE *loc, *pre;
    VERTEX *src, *dst;
    
    //locate src and dst vertices
    src = g->source;
    while(src && (g->compare(src->data, from) > 0 ))
        src = src->next;
    if(!src || (g->compare(src->data, from)!=0))
        return -1;//SRC VERTEX NOT FOUND
    dst = g->source;
    while(dst && (g->compare(dst->data, to) > 0))
        dst = dst->next;
    if(!dst || (g->compare(dst->data, to)!=0))
        return -2;
    //locate edge in adjacency list of SRC vertex
    if(!src->edge_list)
        return -3;
    loc = src->edge_list;
    pre = NULL;
    while(loc && (g->compare(loc->dest->data, to) > 0))
    {
        pre = loc;
        loc = loc->next;
    }
    if(!loc || (g->compare(loc->dest->data, to)!=0))
        return -4;
    if(!pre)//@ front
        src->edge_list = src->edge_list->next;
    else//@ middle or end
        pre->next = pre->next->next;
    free(loc);
    //NOW THE OTHER WAY
    if(!dst->edge_list)
        return -5;
    loc = dst->edge_list;
    pre = NULL;
    while(loc && (g->compare(loc->dest->data, from) > 0))
    {
        pre = loc;
        loc = loc->next;
    }
    if(!loc || (g->compare(loc->dest->data, from)!=0))
        return -4;
    if(!pre)//@ front
        dst->edge_list = dst->edge_list->next;
    else//@ middle or end
        pre->next = pre->next->next;
    free(loc);
    return 1;
}

void traverse_directed_list(GRAPH *g)
{
    //PRINT OUT OF ALL VERTICES AND ADJ
    VERTEX *v;
    ARC *a;
    
    v = g->source;
    while(v)
    {
        printf("(");
        g->process(v->data);
        printf(")");
        if(v->adj_list)
        {
            printf(":");
            a = v->adj_list;
            while(a)
            {
                g->process(a->dest->data);
                if(g->w == IS_WEIGHTED)
                    printf("(%d) ", a->weight);
                a = a->next;
            }
        }
        printf("\n");
        v = v->next;
    }
}

void traverse_undirected_list(GRAPH *g)
{
    //PRINT OUT OF ALL VERTICES AND ADJ
    VERTEX *v;
    EDGE *e;
    
    v = g->source;
    while(v)
    {
        printf("(");
        g->process(v->data);
        printf(")");
        if(v->edge_list)
        {
            printf(": ");
            e = v->edge_list;
            while(e)
            {
                g->process(e->dest->data);
                if(g->w == IS_WEIGHTED)
                    printf("(%d) ", e->weight);
                e = e->next;
            }
        }
        printf("\n");
        v = v->next;
    }
}


void delete_list_directed_graph(GRAPH *g)
{
    VERTEX *pre, *cur;
    ARC *p, *c;
    
    pre = cur = g->source;
    while(cur)
    {
        if(cur->adj_list)
        {
            p = c = cur->adj_list;
            while(c)
            {
                c = c->next;
                free(p);
                p = c;
            }
        }
        cur = cur->next;
        free(pre);
        pre = cur;
    }

}

void delete_list_undirected_graph(GRAPH *g)
{
    VERTEX *pre, *cur;
    EDGE *p, *c;
    
    pre = cur = g->source;
    while(cur)
    {
        if(cur->adj_list)
        {
            p = c = cur->edge_list;
            while(c)
            {
                c = c->next;
                free(p);
                p = c;
            }
        }
        cur = cur->next;
        free(pre);
        pre = cur;
    }

}


/*
for arc
*/
void depth_first_list_digraph_traversal(GRAPH *g)
{
    VERTEX *v;
    ARC *a;
    
    STACK *s = create_stack_adt(g->process);
    //preprocessing
    v = g->source;
    while(v)
    {
        v->processed = 0;
        v = v->next;
    }
    push_stack_adt(s, g->source);
    g->source->processed = 1;//pushed
    while(!is_empty_stack_adt(s))
    {
        v = pop_stack_adt(s);
        g->process(v->data);
        v->processed = 2;
        //process it's adjacency list
        if(v->adj_list)
        {
            a = v->adj_list;
            while(a)
            {
                if(a->dest->processed < 1)
                {
                    push_stack_adt(s, a->dest);
                    a->dest->processed = 1;
                }
                a = a->next;
            }
        }
        v = v->next;
    }
    //destroy the stack
}

/*
for edge
*/
void depth_first_list_graph_traversal(GRAPH *g)
{
    VERTEX *v;
    EDGE *e;
    
    STACK *s = create_stack_adt(g->process);
    //preprocessing
    v = g->source;
    while(v)
    {
        v->processed = 0;
        v = v->next;
    }
    push_stack_adt(s, g->source);
    g->source->processed = 1;//pushed
    while(!is_empty_stack_adt(s))
    {
        v = pop_stack_adt(s);
        g->process(v->data);
        v->processed = 2;
        //process it's adjacency list
        if(v->edge_list)
        {
            e = v->edge_list;
            while(e)
            {
                if(e->dest->processed < 1)
                {
                    push_stack_adt(s, e->dest);
                    e->dest->processed = 1;
                }
                e = e->next;
            }
        }
        v = v->next;
    }
    //destroy the stack
}

void breadth_first_list_digraph_traversal(GRAPH *g)
{
    VERTEX *v;
    ARC *a;
    QUEUE *q;
    
    if(g->count == 0)
        return;
    
    v = g->source;
    while(v)
    {
        v->processed = 0;
        v = v->next;
    }
    q = create_queue_adt(g->process);
    enqueue_queue_adt(q, g->source);
    g->source->processed = 1;
    while(!is_queue_empty_queue_adt(q))
    {
        v = deque_queue_adt(q);
        g->process(v->data);
        v->processed = 2;
        //process it's adjacency list
        if(v->adj_list)
        {
            a = v->adj_list;
            while(a)
            {
                if(a->dest->processed < 1)
                {
                    enqueue_queue_adt(q, a->dest);
                    a->dest->processed = 2;
                }
                a = a->next;
            }
        }
        v = v->next;
    }
        printf("\n");
    //destroy the queue
}

void breadth_first_list_graph_traversal(GRAPH *g)
{
    VERTEX *v;
    EDGE *e;
    QUEUE *q;
    
    v = g->source;
    while(v)
    {
        v->processed = 0;
        v = v->next;
    }
    q = create_queue_adt(g->process);
    enqueue_queue_adt(q, g->source);
    g->source->processed = 1;
    while(!is_queue_empty_queue_adt(q))
    {
        v = deque_queue_adt(q);
        g->process(v->data);
        v->processed = 2;
        //process it's adjacency list
        if(v->edge_list)
        {
            e = v->edge_list;
            while(e)
            {
                if(e->dest->processed < 1)
                {
                    enqueue_queue_adt(q, e->dest);
                    e->dest->processed = 2;
                }
                e = e->next;
            }
        }
        v = v->next;
    }
    printf("\n");
    //destroy the queue
}


/**** APPLICATIONS AND ALGORITHMS:
MST, DIJKSTRA, PRIM, KRUSKAL, ETC.
***/
void create_set_of_edges(GRAPH *g, int *e_count)
{
    VERTEX *v;
    EDGE *a;
    EDGE_ELEMENT *e, *pre, *loc;
    *e_count = 0;
    v = g->source;
    while(v)
    {
        if(v->edge_list)
        {
            a = v->edge_list;
            while(a)
            {
                e = (EDGE_ELEMENT*)malloc(sizeof(EDGE_ELEMENT));//e: (v)------(a->dest)
                e->src = v;
                e->dst = a->dest;
                e->weight = a->weight;
                if(*e_count == 0)
                {
                    g->E = e;
                    e->next = NULL;
                }
                else
                {
                    pre = NULL; loc = g->E;
                    while(loc && (e->weight > loc->weight))
                    {
                        pre = loc;
                        loc = loc->next;
                    
                    }
                    if(!pre)
                        g->E = e;
                    else
                        pre->next = e;
                     e->next = loc;
                }
                (*e_count)++;
                a = a->next;
            }//while adj list
        
        }
        v = v->next;
    }//while vs
}

void print_set_of_edges(GRAPH *g)
{
    for(EDGE_ELEMENT *e = g->E; e; e = e->next)
    {
        g->process(e->src->data);printf("---");
        printf("(%d)", e->weight);printf("---");
        g->process(e->dst->data);printf("\n");
    }
}

void create_set_of_arcs(GRAPH *g, int *e_count)
{
    VERTEX *v;
    ARC *a;
    ARC_ELEMENT *e, *pre, *loc;
    *e_count = 0;
    v = g->source;
    while(v)
    {
        if(v->adj_list)
        {
            a = v->adj_list;
            while(a)
            {
                e = (ARC_ELEMENT*)malloc(sizeof(ARC_ELEMENT));//e: (v)------(a->dest)
                e->src = v;
                e->dst = a->dest;
                e->weight = a->weight;
                if(*e_count == 0)
                {
                    g->A = e;
                    e->next = NULL;
                }
                else
                {
                    pre = NULL; loc = g->A;
                    while(loc && (e->weight > loc->weight))
                    {
                        pre = loc;
                        loc = loc->next;
                    
                    }
                    if(!pre)
                        g->A = e;
                    else
                        pre->next = e;
                     e->next = loc;
                }
                (*e_count)++;
                a = a->next;
            }//while adj list
        
        }
        v = v->next;
    }//while vs
}

void print_set_of_arcs(GRAPH *g)
{
    for(ARC_ELEMENT *e = g->A; e; e = e->next)
    {
        g->process(e->src->data);printf("---");
        printf("(%d)", e->weight);printf("---");
        g->process(e->dst->data);printf("\n");
    }
}


#include "forest.h"

/*
use a queue to process from souce to all branches etc.
*/
void print_forest(FOREST *f, void (*process)(void *data))
{
    QUEUE *q;
    TREE *t;
    
    q = create_queue_adt(NULL);
    enqueue_queue_adt(q, f->ary[0]);
    f->ary[0]->processed = 1;
    while(!is_queue_empty_queue_adt(q))
    {
        t = deque_queue_adt(q);
        process(t->data);
        t->processed = 2;
        if(t->count > 1)//has children
        {
            for(int j = 0; j < t->count - 1; j++)
            {
                if(t->branch_out[j]->processed < 1)
                {
                    enqueue_queue_adt(q, t->branch_out[j]);
                    t->branch_out[j]->processed = 1;
                }
        
            }
        }
    }
}

void mst_krusal(GRAPH *g)
{
    int e_count;
    FOREST *f;
    VERTEX *v;

    //SORTED EDGES ASCENDING
    create_set_of_arcs(g, &e_count);//treat as edges ignoring directions and looking only at the link and weight.
    print_set_of_arcs(g);

    //CREATE COLLECTION OF TREES -- FOREST WHICH WILL EVENTUALLY BECOME AN MST
    f = create_forest(g->count, g->compare);
    v = g->source;
    for(int i = 0; i < g->count; i++)
    {
        f->ary[i]->data = v->data;
        f->ary[i]->processed = 0;
        v = v->next;
    }
    //TEST PRINT FOREST:
    for(int i = 0; i < f->count; i++)
        printf("%p\n", f->ary[i]);
    
    ///TRAVERSE SET OF EDGES
    TREE *ta, *tb;
    for(ARC_ELEMENT *arc = g->A; arc && !f->mst_complete; arc = arc->next)
    {
        //test endpoints are not in same tree
        ta = find(f->ary, f->count, arc->src->data);
        tb = find(f->ary, f->count, arc->dst->data);
        if(ta != tb)
        {
            //1) make unions if possible
            printf("edge cost %d:\n", arc->weight);
            g->process(arc->src->data);printf("---");g->process(arc->dst->data);
            printf("\n");
            make_union(f, arc->src->data, arc->dst->data, arc->weight);
            //2) add to the MST FOREST
        }
    }
    printf("MST COST: %d\n", f->mst_cost);
    puts("PRINT SOLUTION:::");
    //MST COMPLETE PRINT IT OUT:
    //print_forest(f, g->process);
    QUEUE *q;
    TREE *t;
    
    q = create_queue_adt(g->process);
    enqueue_queue_adt(q, f->ary[0]);
    f->ary[0]->processed = 1;
    while(!is_queue_empty_queue_adt(q))
    {
        t = deque_queue_adt(q);
        printf("");
        g->process(t->data);
        t->processed = 2;
        if(t->count > 1)//has children
        {
            printf(" CONNECTS TO: ");
            for(int j = 0; j < t->count - 1; j++)
            {
                //printf("(%d) ", t->branch_out[j]->w);
                printf("\n---");
                g->process(t->branch_out[j]->data);
                printf("(%d)", t->w_ary[j]);
                printf("---\n");
                if(t->branch_out[j]->processed < 1)
                {
                    enqueue_queue_adt(q, t->branch_out[j]);
                    t->branch_out[j]->processed = 1;
                }
            }
            printf("\n");
        }
    }
    
}

int mst_compare(void *a, void *b)
{
    VERTEX *a1, *b1;
    
    a1 = (VERTEX*)a;
    b1 = (VERTEX*)b;
    
    if(a1->key < b1->key)
        return -1;
    else if (a1->key > b1->key)
        return +1;
    else
        return 0;
}

int mst_compare2(void *a, void *b)
{
    VERTEX *a1, *b1;
    
    a1 = (VERTEX*)a;
    b1 = (VERTEX*)b;
    
    if(a1->d < b1->d)
        return -1;
    else if (a1->d > b1->d)
        return +1;
    else
        return 0;
}

void dijkstra(GRAPH *g)
{
    HEAP_ADT* Q;
    VERTEX *u;
    ARC *v;
    
    //INITIALIZE
    Q = heap_adt(g->count, mst_compare2);
    u = g->source;
    u->d = 0;
    u->pi = NULL;
    Q->ary[0] = u;
    u = u->next;
    Q->count++;
    u->in_msp = 0;
    while(u)
    {
        u->d = INT_MAX; //distance
        u->pi = NULL; //predecessor
        u->in_msp = 0;
        Q->ary[Q->count] = u;
        Q->count++;
        u = u->next;
    }
    build_min_heap(Q);
    
    //PRIORITY QUEUE FROM BINARY HEAP
    while(Q->size > 0)
    {
        u = get_min(Q);
        u->in_msp = 1;
        puts("MIN");
        g->process(u->data);puts("\n");
        if(u->adj_list)
        {
            v = u->adj_list;
            while(v)
            {
                if(v->dest->in_msp == 0 && (v->dest->d > u->d + v->weight)) // v.d > v.d + w(u, v)
                {
                    puts("ADJ:");
                    g->process(v->dest->data);puts("\n");
                    v->dest->d = u->d + v->weight;
                    v->dest->pi = u;
                }
                v = v->next;
            }
            build_min_heap(Q);//rebuild after we update
        }
    }//
    printf("\nDIJKSTRA\n");
    for(u = g->source; u; u = u->next)
    {
        g->process(u->data);
        printf("%d ", u->d);
        printf("pi: ");
        g->process(u->data);
        printf("\n");
    }
}
#endif /* graph_h */
