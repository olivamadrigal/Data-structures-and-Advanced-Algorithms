#include "dynamic_arrays.h"
#include "arrays.h"
#include "linked_list.h"
#include "ht_linked_list.h"
#include "c_linked_list.h"
#include "d_linked_list.h"
#include "queue.h"
#include "stack.h"
#include "graph.h"
#include "heap.h"
#include "hash_table.h"
#include "bst.h"
#include "sorting.h"

/***** DATA STRUCTURES USING ABSTRACT DATA TYPES with sample application data structures
This material is intended for educational purposes, such as teaching or review.
It is execellent supplement for teaching as it explores the different implementations or fundamental
data structures with concrete simulations.
It is also an excellent programming language as it ensures solid understanding of abstract data types,
memory (heap and stack), and pointers. Programming in C++ or other object oriented languages like Python
is quite natural.
##################
FIRST PART: ARRAYS (FIXED AND DYNAMIC), LINKED LISTS, QUEUES, STACKS
SECOND PART: GRAPHS, HEAP, HASHING TABLES, BST, & SORTING.

EXERCISES: GENERAL TREES, AVL, RED-BLACK TREE, B-TREES (aka M-way trees), sorting in linear time (radix etc.)
MORE SPECIALIZED DATA STRUCTURES: MERKLE TREES (hash trees), TRIES, etc.
##################
complexity overview:
Heap: O(1) get min/max; del/insert O(log(n)); build O(nlog(n)).
Binary tree has O(log(n)) but at worst can be like a linear list...
AVL O(log(n)) vs RB trees: AVL has faster lookup due to strict balancing, height = 1.44log_2(n+1)
RB tree: allows faster insert/delete because it has less rotations, relaxed balancing. has max height = 2log(n+1) (rearrange + repaint)
B-tree: O(log(n)) and it is specific data structure for database applications, disk specific, such a perfectly balanced m-way tree
(m branches, m-1 data entries) will save a factor of log(t) for t = m, in the number of nodes which are examined.
Graph- Matrix: insert/del O(1) for eddes, O(v^2) for vertices, and space (V^2)
Linked list version: insert/del O(E+1) for edges, insert/del O(V+1) for vertices, and space (V + E)
Hashing:  O(1) look ups.
DFT uses stack, moves vertically, O(E+V)time and O(V)space.
BFT uses queue, moves horizontally, O(E+V)time and O(V)space.
SSSP Dijsktra has O(V+E)log(V)) for binary heap implementation.
TRIES: fast prefix lookup O(n) on string length and can leaverage.
****/
/*** for simplicity: all functions prototypes and source code in .h lib file as opposed to
function prototypes in .h file and functions definitions in .c.
Input file names taken from Turkish dramas :))))
***/

/*** Some of this work adapts, modifies, and makes reference from algorithms and problems from:
[1] https://mitpress.mit.edu/books/introduction-algorithms-third-edition
[2] https://books.google.com/books/about/Data_Structures_A_Pseudocode_Approach_wi.html?id=JTU9AAAAQBAJ
[3]https://www.google.com/books/edition/Computer_Science/A4chAQAAIAAJ?hl=en&gbpv=1&bsq=computer+science+forouzan&dq=computer+science+forouzan&printsec=frontcover
***/

//PERSON STRUCUTE -- your class object analog.
typedef struct person
{
    char *first;
    char *last;
    int ssn;
    int age;
}PERSON;

//ROUTER OBJECT
typedef struct router
{
    char name;
    uint32_t ip_addr;
    int reachability;
}ROUTER;

typedef struct route_24
{
    uint8_t outgoing_intf;
}ROUTE_24;

typedef struct ipv4_addr
{
    uint32_t ip_address;
    uint32_t subnet_mask;
    uint32_t network_address;
    uint8_t prefix_len;
}IPv4_ADDR;

typedef struct page
{
    int valid;
    int ppn;//physical page number
    
}PAGE;//array of thes, direct hashing using virtual addresses.

typedef struct virtual_address
{
    int vpn;
    int offset;
    
}VIRTUAL_ADDRESS;

typedef enum {GLOBAL=1,STATIC=2,LOCAL=3}SCOPE;
typedef enum {VARIABLE=1,FUNCTION_NAME=2,CONSTANT=3,COMPILER_TEMP=4}ENTRY_TYPE;
typedef struct sym
{
    char *name;
    SCOPE var_scope;
    ENTRY_TYPE var_type;
    int hash_key;
}SYM;

typedef struct employee
{
    char *first;//first name
    unsigned int EN;//employee number
    //etc.
}EMPLOYEE;

uint32_t PREFIX_BK[10];//GLOBAL

//APPLICATION-SPECIFIC FUNTIONS
int compare(void *arg1, void *arg2);
void process(void *arg2);

//PRE PROCESSING FUNCTIONS
PERSON **process_input_file(char *input_file, int size);
int get_line_count(char *file_name);

//LINEAR DATA STRUCTURES
void sample_dynamic_array(PERSON **ary, int size);
void sample_fixed_array(PERSON **ary, int size);
void sample_linked_list(PERSON **ary, int size);//singly linked list
void sample_ht_linked_list(PERSON **ary, int size);//header trailer linked list
void sample_c_linked_list(PERSON **ary, int size);//circulr linked list
void sample_d_linked_list(PERSON **ary, int size);//doubly linked list
void sample_queue(PERSON **ary, int size);//queue
void sample_stack(PERSON **ary, int size);//stack
void create_sample_topology(char *vertex_file, char *links_file, GRAPH *g, ROUTER *nodes[]);
void sample_graph_matrix_version(char *vertex_file, char *links_file, int size);
int32_t string_to_binary_ip(char *dot_addr);
void sample_graph_linked_list_version(char *vertex_file, char *links_file, int size);
GRAPH *sample_digraph_linked_list_version(char *vertex_file, char *links_file, int size);
void create_interrupt_vector(AHEAP *header, char *intr_vectors);
void create_interrupt_vector_array(char *intr_vectors, INTR **iv);
int h1_compare(void *arg1, void *arg2);
void h1_process(void *data);
void sample_fixed_heap(char *intr_vector);
void sample_dynamic_heap(char *intr_vector);
void sample_ptrbased_heap(char *intr_vector);
void sample_array_ht(void);
ROUTE_24** testing_fwd_table_p24(int intf_count);
IPv4_ADDR *addr_32_bit(uint8_t ip_addr[4], int prefix);
int prefix24_hash(uint32_t network_address);
PAGE* create_test_page_table(int size);
void hash_process(void *page_table_data, void *virtual_address);
int hash_function(void *data);
VIRTUAL_ADDRESS *create_test_va_array(int size);
void sample_ht_array_with_collision_resol(char *symbol_table);
void sample_hashed_table_with_bucket(char *symbol_table, int bucket_size);
void sample_hashed_table_with_chaining(char *db_in);
void sample_ht_with_route_fwd_table(char *in);
void bstprocess(void *a);
int bstcompare(void *data_in, void *root);
void sample_bst(char *in);
void sample_sorting_algorithms(void);


//all this is ran in XCODE. Ensure path is correct for input files relative to your test environment.
#define LINKED_LIST_INPUT "input_data1.in"
#define DATA_INPUT2 "input_data2.in"
#define DATA_INPUT3 "input_data3.in"
#define TOPOLOGY_LINKS "topology_links.in"
#define DATA_INPUT4  "input_data4.in"
#define DATA_INPUT5 "sym_table_entries.in"
#define DATA_INPUT6 "org_empl_db.in"
#define DATA_INPUT7 "route_table_test.in"

#define MAX_LINE 80
#define FLUSH while(getchar() != '\n')

int main(int argc, const char * argv[])
{
    
    //####### FIRST HALF CONSISTS OF LINEAR LISTS arrays and pointer-based. #########
    //sample data for realloc use
    /*
    size = get_line_count(DATA_INPUT2);
    PERSON **ary = process_input_file(DATA_INPUT2, size);
    sample_dynamic_array(ary, size);
    free(ary);
    */
    //int size = get_line_count(LINKED_LIST_INPUT);
    //PERSON **ary = process_input_file(LINKED_LIST_INPUT, size);
    /* ARRAYS HAVE FAST RANDOM ACCESS */
    //sample_fixed_array(ary, size);
    /*  LISTS HAVE FAST INSERT/DELETE */
    /*  LINKED LISTS: O(1) + SEARCH O(n) worst. */
    //sample_linked_list(ary, size);
    //sample_ht_linked_list(ary, size);
    //sample_c_linked_list(ary, size);
    //sample_d_linked_list(ary, size);
    /* QUEUES AND STACKS: O(1) insert/delete    */
    //sample_queue(ary, size);
    //sample_stack(ary, size);
    //free(ary);
    
     //SECOND HALF
     /* GRAPH */
     //mini topology testbed -- MATRIX VERSION
     /*
     int size = get_line_count(DATA_INPUT3);
     //sample_graph_matrix_version(DATA_INPUT3, TOPOLOGY_LINKS, size);
     //sample_graph_linked_list_version(DATA_INPUT3, TOPOLOGY_LINKS, size);
     GRAPH *g = sample_digraph_linked_list_version(DATA_INPUT3, TOPOLOGY_LINKS, size);
     puts("GREEDY ALGORIHTMS:");
     puts("MST--- KRUSKAL");
     mst_krusal(g);
     puts("SSSP --- DIJKSTRA ON DIGRAPH");
     dijkstra(g);
     */
    /*
    recall we are running dijkstra on a digraph: where $ == --> to arrow.
    
    (u)----[1]----$(v)$
    |               $  \
    |               |   \[6]
    [2]            [3]    \
    |               |       (z)
    |               |      /$
    $               |   /[2]
    (y)----[3]----$(x)/
    */
    /*
     Minimum spanning tree looks like this:
    (u)+ + + [1]+ + + (v)
     +                +
     +                +     (z)
    [2]              [3]    +
     +                +  [2]
     +                + +
    (y)              (x)
    (for visualization, we leave only edges which makeup the tree)
    
    //BINARY HEAPS
    */
    //FIXED ARRAY
    //BINARY HEAP: useful whenever we want to smallest/largest element FAST.
    //sample_fixed_heap(DATA_INPUT4);
    //DYNAMIC ARRAY
    //sample_dynamic_heap(DATA_INPUT4);
    //if you draw out the heap you can esily see it keeps the property
    //left and right subtrees are less than or equal to the parent.
    /*
     write out all the indecis like in breadth first traversal
     then fill in their (data). index(data):
     
                0(18)
            1(14)    2(12).... etc.
          3     4   5    6
        7     9
    */
    //POINTER-BASED HEAP
    //sample_ptrbased_heap(DATA_INPUT4);

    //HASH TABLES:  ARRAYS, BUCKETS, & CHAINING
    //1) DIRECT ADDRESSING WITH PAGE TABLE
    //sample_array_ht();
    // 2) SIMULATED a mini forwarding table of 5 entries -- will hash all /24 addresses
    // to some outgoing interface based on the set ranges populated by the routing algorithm(s) used.
    // ROUTING TABLE = AN ARRAY OF HASHTABLES
    //sample_ht_array_with_collision_resol(DATA_INPUT5);
    //3) HASHED TABLE WITH BUCKET RESOLUTION
    //sample_hashed_table_with_bucket(DATA_INPUT5,3);
    //4) HASHED TABLE WITH CHAINING
    //sample_hashed_table_with_chaining(DATA_INPUT6);
    //5) sample route fwd table
    //sample_ht_with_route_fwd_table(DATA_INPUT7);
    //BINARY TREES
    //sample_bst(LINKED_LIST_INPUT);
    //SORTING
    sample_sorting_algorithms();
    return 0;
}

int compare(void *arg1, void *arg2)
{
    PERSON *a, *b;
    a = (PERSON*)arg1;
    b = (PERSON*)arg2;
    if(a->ssn > b->ssn)
        return +1;
    else if (a->ssn < b->ssn)
        return -1;
    else
        return 0;
}

void process(void *data)
{
    PERSON *p = (PERSON*)data;
    printf("RECORD:: first:%s last:%s age:%d ssn:%.3d-%.2d-%.3d\n",
    p->first, p->last, p->age,(int)(p->ssn/(int)pow(10.0,5)),(p->ssn%100000)/1000,p->ssn%1000);
}

int get_line_count(char *file_name)
{
    FILE *fp;
    int line_count;
    char buffer[MAX_LINE];
    
    fp = Fopen(file_name, "r");
    line_count = 0;
    while(fgets(buffer, MAX_LINE - 1 , fp))
            line_count++;
    Fclose(fp);
    
    return line_count;
}

PERSON **process_input_file(char *input_file, int size)
{
    PERSON **ary;
    FILE *fp;
    char buffer[MAX_LINE], *token;
    long len;
    int idx;
    
    //process input file and build array of structs
    ary = (PERSON**)Malloc(size*sizeof(PERSON*));
    fp = Fopen(input_file, "r");
    idx = 0;
    while(fgets(buffer, MAX_LINE, fp))
    {
        ary[idx] = (PERSON*)Malloc(sizeof(PERSON));
        token = strtok(buffer, ":");
        len = strlen(token) + 1;
        ary[idx]->first = (char*)Malloc(sizeof(char)*len);
        strncpy(ary[idx]->first, token, len);
        token = strtok(NULL, ":");
        len = strlen(token) + 1;
        ary[idx]->last = (char*)Malloc(sizeof(char)*len);
        strncpy(ary[idx]->last, token, len);
        token = strtok(NULL, ":");
        ary[idx]->age = (int)strtol(token, (char**)NULL, 10);
        token = strtok(NULL, "\n");
        ary[idx]->ssn = (int)strtol(token, (char**)NULL, 10);
        idx++;
    }
    Fclose(fp);
    return ary;
}

void sample_linked_list(PERSON **ary, int size)
{
    puts("########### LINKED LIST SAMPLE ###########\n");
    PERSON *target;
    LL_HEAD *list;

    list = create_linked_list_adt(compare, process);
    //1) build list
    for(int idx = 0; idx < size; idx++)
        insert_to_linked_list_adt(list, ary[idx]);
    
    //2) print the list
    traverse_linked_list_adt(list);
    printf("count = %d\n", get_linked_list_count(list));
   
    //3) delete from list:
    target = (PERSON*)Malloc(sizeof(PERSON));
    puts("\nDELETE FROM FRONT:");
    target->ssn =  11122333;
    delete_from_linked_list_adt(list, target);
    traverse_linked_list_adt(list);
    printf("count = %d\n", get_linked_list_count(list));
    
    puts("\nDELETE AT MIDDLE:");
    target->ssn = 22255333;
    delete_from_linked_list_adt(list, target);
    traverse_linked_list_adt(list);
    printf("count = %d\n", get_linked_list_count(list));
   
    puts("\nDELETE AT REAR:");
    target->ssn = 77788999;
    delete_from_linked_list_adt(list, target);
    traverse_linked_list_adt(list);
    printf("count = %d\n", get_linked_list_count(list));
    
    //4) retrieval
    puts("\nRETRIEVE YIGIT:");
    target->ssn = 22211333;
    process(retrive_from_linked_list_adt(list, target));
    
    //5) delete list
    delete_linked_list_adt(list);
    free(target);
}

void sample_ht_linked_list(PERSON **ary, int size)
{
    puts("########### HEADER TRAILER LINKED LIST SAMPLE ###########\n");
    PERSON *target;
    HTLL_HEAD *list;
    
    //list header
    list = create_ht_linked_list_adt(compare, process);
    //1)build list
    for(int idx = 0; idx < size; idx++)
        insert_to_ht_linked_list_adt(list, ary[idx]);
    
   //print the list
    traverse_ht_linked_list_adt(list);
    printf("count = %d\n", get_ht_linked_list_count(list));
    target = (PERSON*)Malloc(sizeof(PERSON));
    puts("\nDELETE FROM FRONT:");
    target->ssn =  11122333;
    delete_from_ht_linked_list_adt(list, target);
    traverse_ht_linked_list_adt(list);
    printf("count = %d\n", get_ht_linked_list_count(list));
    puts("\nDELETE AT MIDDLE:");
    target->ssn = 22255333;
    delete_from_ht_linked_list_adt(list, target);
    traverse_ht_linked_list_adt(list);
    printf("count = %d\n", get_ht_linked_list_count(list));
    puts("\nDELETE AT REAR:");
    target->ssn = 77788999;
    delete_from_ht_linked_list_adt(list, target);
    traverse_ht_linked_list_adt(list);
    printf("count = %d\n", get_ht_linked_list_count(list));
    puts("\nRETRIEVE YIGIT:");
    target->ssn = 22211333;
    process(retrive_from_ht_linked_list_adt(list, target));
    delete_ht_linked_list_adt(list);
    free(target);
}


void sample_c_linked_list(PERSON **ary, int size)
{
    puts("########### CIRCULAR SINGLY LINKED LIST SAMPLE ###########\n");
    PERSON   *target;
    CLL_HEAD *list;
    
    //list header
    list = create_c_linked_list_adt(compare, process);
    //1) file input data processing & build list
    for(int idx = 0; idx < size; idx++)
    {
        insert_to_c_linked_list_adt(list, ary[idx]);
        traverse_c_linked_list_adt(list);
        printf("count = %d\n", get_c_linked_list_count(list));
    }

   //print the list
    puts("\nREPRINT LIST AFTER PROCESSING\n");
    traverse_c_linked_list_adt(list);
    printf("count = %d\n", get_c_linked_list_count(list));
   
   
    target = (PERSON*)Malloc(sizeof(PERSON));
    puts("\nAFTER DELETE FROM FRONT:");
    target->ssn =  11122333;
    delete_from_c_linked_list_adt(list, target);
    traverse_c_linked_list_adt(list);
    printf("count = %d\n", get_c_linked_list_count(list));
    
    puts("\nAFTER DELETE AT MIDDLE:");
    target->ssn = 55599111;
    delete_from_c_linked_list_adt(list, target);
    traverse_c_linked_list_adt(list);
    printf("count = %d\n", get_c_linked_list_count(list));
    
    puts("\nAFTER DELETE AT REAR:");
    target->ssn = 77788999;
    delete_from_c_linked_list_adt(list, target);
    traverse_c_linked_list_adt(list);
    printf("count = %d\n", get_c_linked_list_count(list));
    
    puts("\nRETRIEVE YIGIT:");
    target->ssn = 22211333;
    process(retrive_from_c_linked_list_adt(list, target));
    puts("\nDELETE REST OF LIST:");
    delete_c_linked_list_adt(list);
    free(target);
    
}

void sample_d_linked_list(PERSON **ary, int size)
{
    puts("########### DOUBLY LINKED LIST SAMPLE ###########\n");
    PERSON *target;
    
    //1) build list
    DL_HEAD *list = create_d_linked_list_adt(compare, process);
    for(int idx = 0; idx < size; idx++)
    {
        insert_to_d_linked_list_adt(list, ary[idx]);
        //2)traverse as we build
        traverse_d_linked_list_adt(list);
        printf("count = %d\n", get_d_linked_list_count(list));
    }
   
   //2) print the list
    puts("\nREPRINT LIST AFTER PROCESSING\n");
    traverse_d_linked_list_adt(list);
    printf("count = %d\n", get_d_linked_list_count(list));
   
   //3) deletion & traversal
    target = (PERSON*)Malloc(sizeof(PERSON));
    puts("\nAFTER DELETE FROM FRONT:");
    target->ssn =  11122333;
    delete_from_d_linked_list_adt(list, target);
    traverse_d_linked_list_adt(list);
    printf("count = %d\n", get_d_linked_list_count(list));
    
    puts("\nAFTER DELETE AT MIDDLE:");
    target->ssn = 22255333;
    delete_from_d_linked_list_adt(list, target);
    traverse_d_linked_list_adt(list);
    printf("count = %d\n", get_d_linked_list_count(list));
    puts("\nAFTER DELETE AT REAR:");
    target->ssn = 77788999;
    delete_from_d_linked_list_adt(list, target);
    traverse_d_linked_list_adt(list);
    printf("count = %d\n", get_d_linked_list_count(list));
    puts("\nAFTER RETRIEVE YIGIT:");
    target->ssn = 22211333;
    process(retrive_from_d_linked_list_adt(list, target));
    //4)delete list
    delete_d_linked_list_adt(list);
    free(target);
}

void sample_queue(PERSON **ary, int size)
{
    puts("########### SAMPLE QUEUE ###########\n");
    PERSON *target;
    QUEUE *q;
    
    q = create_queue_adt(process);
    //1) build list
    for(int idx= 0; idx < size; idx++)
        enqueue_queue_adt(q, ary[idx]); //2) enque
    //3) print queue
    print_queue_adt(q);
    // utility
    printf("count = %d\n", get_count_queue_adt(q));
    puts("PROCESS THE LINE (FIFO):");
    //4) dequeue
    while(!is_queue_empty_queue_adt(q))
    {
        target = deque_queue_adt(q);
        q->process(target);
    }
    //5) destroy queue
    delete_queue_adt(q);
}

void sample_stack(PERSON **ary, int size)
{
    puts("########### SAMPLE STACK ###########\n");
    PERSON *target;
    STACK *s;

    s = create_stack_adt(process);
    for(int idx = 0; idx < size; idx++) //1) build list
        push_stack_adt(s, ary[idx]);
    puts("PRINT LIST IN REVERSE (LIFO):");
    while(!is_empty_stack_adt(s))
    {
        target = pop_stack_adt(s);
        s->process(target);
    }
    destroy_stack_adt(s);
}

void sample_fixed_array(PERSON **ary, int size)
{
    puts("########### SAMPLE FIXED-SIZE ARRAY ###########\n");
    ARY_HEADER *header;
    PERSON *target;
    
    //build array
    header = create_array_adt(size, process, compare);
    for(int i = 0; i < size; i++) //1) build list
    {
         insert_to_array_adt(header, ary[i]);
         //puts("INSERT: \n");
         //traverse_array_adt(header);

    }
    //2) traverse array
   traverse_array_adt(header);
    //3) FAST <RANDOM> ACCESS:
    puts("FAST RANDOM ACCESS: indexing cost is 0(1)");
    for(int i = 0; i < size; i++)
        process(retrieve_from_array_adt(header, i));
    puts("FAST RANDOM ACCESS: indexing using test pRNG from standard C library to generate indices");
    for(int i = 0; i < size; i++)
        process(retrieve_from_array_adt(header, rand() % size));
    puts("DELETE like INSERT: worst 0(1) + O(n) for search and requires shifting elements\n");
    puts("DELETE @ front (Nesfar): \n");
    target = (PERSON*)Malloc(sizeof(PERSON));
    target->ssn = 11122333;
    delete_from_array_adt(header, target);
    if(!header->ary[header->count])
        puts("LAST SLOT NOW NULL");
    traverse_array_adt(header);
    puts("\nDELETE AT MID (Massar):");
    target->ssn = 22255333;
    delete_from_array_adt(header, target);
    if(!header->ary[header->count])
        puts("LAST SLOT NOW NULL");
    traverse_array_adt(header);
    puts("\nDELETE AT END (Mercan):");
    target->ssn = 77788999;
    delete_from_array_adt(header, target);
    if(!header->ary[header->count])
        puts("LAST SLOT NOW NULL");
    traverse_array_adt(header);
    destroy_array_adt(header);
}


void sample_dynamic_array(PERSON **ary, int size)
{
    puts("########### SAMPLE DYNAMIC ARRAY ###########\n");
    DARY_HEADER *header;
    PERSON *target;
    int first;
    
    first = size/2;
    header = create_darray_adt(first, process, compare);
    //process 1st half
    for(int i = 0; i < first; i++)
         insert_to_darray_adt(header, ary[i]);
    puts("FIRST HALF\n");
    traverse_darray_adt(header);
    //second hald
    for(int i = first; i < size; i++)
         insert_to_darray_adt(header, ary[i]);
    puts("SECOND HALF\n");
    traverse_darray_adt(header);
     //3) FAST <RANDOM> ACCESS:
    puts("FAST RANDOM ACCESS: indexing cost is 0(1)");
    for(int i = 0; i < size; i++)
        process(retrieve_from_darray_adt(header, i));
    puts("FAST RANDOM ACCESS: indexing using test pRNG from standard C library to generate indices");
    for(int i = 0; i < size; i++)
        process(retrieve_from_darray_adt(header, rand() % size));
    puts("DELETE like INSERT: worst 0(1) + O(n) for search and requires shifting elements\n");
    puts("DELETE @ front (Osman): \n");
    target = (PERSON*)Malloc(sizeof(PERSON));
    target->ssn = 00022444;
    delete_from_darray_adt(header, target);
    if(!header->ary[header->count])
        puts("LAST SLOT NOW NULL");
    traverse_darray_adt(header);
    puts("\nDELETE AT MID (Massar):");
    target->ssn = 22255333;
    delete_from_darray_adt(header, target);
    if(!header->ary[header->count])
        puts("LAST SLOT NOW NULL");
    traverse_darray_adt(header);
    puts("\nDELETE AT END (Esma):");
    target->ssn = 99922333;
    delete_from_darray_adt(header, target);
    if(!header->ary[header->count])
        puts("LAST SLOT NOW NULL");
    traverse_darray_adt(header);
    destroy_darray_adt(header);
    
}

int gcompare(void *arg1, void *arg2)
{
    ROUTER *r1, *r2;
    
    r1 = (ROUTER*)arg1;
    r2 = (ROUTER*)arg2;
    
    if(r1->ip_addr == r2->ip_addr)
        return 0;
    else
        return -1;
}

int glcompare(void *arg1, void *arg2)
{
    ROUTER *r1, *r2;
    
    r1 = (ROUTER*)arg1;
    r2 = (ROUTER*)arg2;
    
    //printf("%d: %d\n", r1->ip_addr, r2->ip_addr);
    
    if(r1->reachability > r2->reachability)
        return +1;
    else if(r1->reachability < r2->reachability)
        return -1;
    else
        return 0;
}

void gprocess(void *data)
{
    ROUTER *r;
    
    r = (ROUTER*)data;
    printf("%c ", r->name);

}

int32_t string_to_binary_ip(char *in_addr)
{
    int32_t ip_addr;
    uint8_t octs[4];
    char *ptr, *dot_addr;
    int i, len;
    //dotted decimal string ---> octects; atoi etc required fixed-array or malloced array.
    len = (int)strlen(in_addr)+1;
    dot_addr = (char*)Malloc(sizeof(char)*len);
    strncpy(dot_addr, in_addr, len);
    for(i = 0, ptr = strtok(dot_addr, "."); ptr; ptr = strtok(NULL, "."), i++)
        octs[i] = (uint8_t)strtol(ptr,(char**)NULL, 10);
    //CONVERT TO BINARY 32-BIT COMPUTER ADDRESS
    ip_addr = 0;
    for(int i = 0; i < 4; i++)
        ip_addr = ip_addr*256 + octs[i];
    free(dot_addr);
    return ip_addr;
}


/*
build sample topology:
//u ---(1)--- v
//u ---(2)--- y
//y ---(3)--- x
//x ---(3)--- v
//x ---(2)--- z
//z ---(6)--- v
    (u)---[1]----(v)
     |          |\
     |          | \[6]
    [2]        [3]  \(z)
     |          |  /[2]
    (y)--[3]---(x)/
*/
void create_sample_topology(char *vertex_file, char *links_file, GRAPH *g, ROUTER *r[])
{
    ROUTER *rt;
    FILE *fp;
    char buffer[MAX_LINE], *name, *ip_addr;
    int i, src_idx, dst_idx, weight;
    
    i = 0;
    fp = Fopen(vertex_file, "r");
    while(fgets(buffer, MAX_LINE, fp))
    {
        name = strtok(buffer, ":");
        ip_addr = strtok(NULL, ":");//no need to NULL-terminate for strtol
        rt = (ROUTER*)Malloc(sizeof(ROUTER));
        r[i] = rt;
        rt->name = name[0];
        rt->ip_addr = string_to_binary_ip(ip_addr);
        rt->reachability = i+1;
        insert_to_graph(g, rt);
        i++;
    }
    Fclose(fp);
    fp = Fopen(links_file, "r");
    while(fgets(buffer, MAX_LINE, fp))
    {
        src_idx = (int)strtol(strtok(buffer, ","), (char**)NULL, 10);
        dst_idx = (int)strtol(strtok(NULL, ","), (char**)NULL, 10);
        weight = (int)strtol(strtok(NULL, ","), (char**)NULL, 10);
        add_arc_edge_to_graph(g, r[src_idx], r[dst_idx], weight);
    }
    Fclose(fp);
}

void sample_graph_matrix_version(char *vertex_file, char *links_file, int size)
{
    puts("### UNIDIRECTIONAL GRAPH, MATRIX IMPLEMENTATION ####");
    GRAPH  *g;
    ROUTER *nodes[size];//nodes

    g = create_graph(MATRIX, UNDIRECTED, IS_WEIGHTED, size, gcompare, gprocess);
    create_sample_topology(vertex_file, links_file, g, nodes);
    puts("\nTRAVERSE GRAPH\n");
    traverse_matrix(g);
    printf("DEPTH FIRST TRAVERSAL\n");
    depth_first_graph_traversal(g);
    printf("BREADTH FIRST TRAVERSAL\n");
    breadth_first_graph_traversal(g);
    printf("\n");
    printf("SEARCH FOR EACH NODE\n");
    for(int i = 0; i < size; i++)
    {
        if(search_graph(g, nodes[i]))
            printf("FOUND!!!\n");
    }
    puts("DEL EDGE z ---(6)--- v: ");
    del_arc_edge_from_graph(g, nodes[4], nodes[1]);//z ---(6)--- v
    traverse_matrix(g);
    puts("RE-ADD EDGE z ---(6)--- v: ");
    add_arc_edge_to_graph(g, nodes[4], nodes[1], 6);//z ---(6)--- v
    traverse_matrix(g);
    puts("DESTROY GRAPH:\n");
    delete_graph(g);
}

void sample_graph_linked_list_version(char *vertex_file, char *links_file, int size)
{
    puts("### DIGRAPH GRAPH, LINKED-LIST IMPLEMENTATION ####");
    GRAPH  *g;
    ROUTER *nodes[size];//nodes
    
    g = create_graph(ADJACENCY_LIST, UNDIRECTED, IS_WEIGHTED, size, glcompare, gprocess);
    create_sample_topology(vertex_file, links_file, g, nodes);
    puts("\nTRAVERSE DIGRAPH GRAPH\n");
    traverse_graph(g);
    printf("DEPTH FIRST TRAVERSAL\n");
    depth_first_graph_traversal(g);
    printf("\nBREADTH FIRST TRAVERSAL\n");
    breadth_first_graph_traversal(g);
}

GRAPH *sample_digraph_linked_list_version(char *vertex_file, char *links_file, int size)
{
    puts("### DIGRAPH GRAPH, LINKED-LIST IMPLEMENTATION ####");
    GRAPH  *g;
    ROUTER *nodes[size];//nodes
    
    g = create_graph(ADJACENCY_LIST, DIRECTED, IS_WEIGHTED, size, glcompare, gprocess);
    create_sample_topology(vertex_file, links_file, g, nodes);
    puts("\nTRAVERSE DIGRAPH GRAPH\n");
    traverse_graph(g);
    printf("DEPTH FIRST TRAVERSAL\n");
    depth_first_graph_traversal(g);
    printf("\nBREADTH FIRST TRAVERSAL\n");
    breadth_first_graph_traversal(g);
    return g;
}

void create_interrupt_vector(AHEAP *header, char *intr_vectors)
{
    FILE *fp;
    char buffer[80];
    char *describtion, *priority;
    int count;
    
    //BUILD INTERRUPT VECTOR
    fp = Fopen(intr_vectors,  "r");
    count = 0;
    while(fgets(buffer, 80, fp))
    {
        priority = strtok(buffer, ":");
        describtion = strtok(NULL, ":");
        header->intr_vector[count].description = (char*)Malloc((strlen(describtion)+1)*sizeof(char));
        strncpy(header->intr_vector[count].description, describtion, (strlen(describtion)+1));
        header->intr_vector[count].priority = (int)strtol(priority, (char**)NULL, 10);
        count++;
    }
    Fclose(fp);
}

void create_interrupt_vector_array(char *intr_vectors, INTR **iv)
{
    FILE *fp;
    char buffer[80];
    char *describtion, *priority;
    int count;
    
    //BUILD INTERRUPT VECTOR
    fp = Fopen(intr_vectors,  "r");
    count = 0;
    while(fgets(buffer, 80, fp))
    {
        iv[count] = (INTR*)Malloc(sizeof(INTR));
        priority = strtok(buffer, ":");
        iv[count]->priority = (int)strtol(priority, (char**)NULL, 10);
        describtion = strtok(NULL, ":");
        iv[count]->description = (char*)Malloc((strlen(describtion)+1)*sizeof(char));
        strncpy(iv[count]->description, describtion,(strlen(describtion)+1));
        count++;
    }
    Fclose(fp);
}

//APPLICATION-SPECIFIC PROCESSING FUNCTIONS
int h1_compare(void *arg1, void *arg2)
{
    INTR *a, *b;
    
    a = (INTR*)arg1;
    b = (INTR*)arg2;
    
    if(a->priority >  b->priority)
        return +1;
    else if (a->priority <  b->priority)
        return -1;
    else
        return 0;
}

void h1_process(void *data)
{
    INTR *intr_vd;
    intr_vd = (INTR*)data;
    printf("%d:%s\n", intr_vd->priority, intr_vd->description);
}

void sample_fixed_heap(char *intr_vector)
{
    int size;
    AHEAP *my_heap;
    HEAP_TYPE htype;
    IMPLEMENTATION_TYPE itype;
    INTR entry;
    
    size = get_line_count(DATA_INPUT3);
    htype = MIN;
    itype = FIXED_ARRAY;
    my_heap = create_array_heap(size, htype, itype);
    create_interrupt_vector(my_heap, intr_vector);
    build_heap(my_heap, htype);
    //PRINT HEAP
    print_heap(my_heap);
    //GET THE ROOT
    puts("ROOT:\n");
    entry = get_root(my_heap, htype);
    printf("\n\n priority=%d description=%s\n\n\n", entry.priority, entry.description);
    print_heap(my_heap);
    entry.description = "cpu_max_voltage_exceeded";
    entry.priority = 20;
    heap_insert(my_heap, entry, htype);
    print_heap(my_heap);

}

void sample_dynamic_heap(char *intr_vector)
{
    int size, first, amt;
    DHEAP *my_dheap;
    INTR **ivt;
    HEAP_TYPE htype;
    IMPLEMENTATION_TYPE itype;

    htype = MAX;
    itype = DYNAMIC_ARRAY;
    amt = 2;
    size = get_line_count(intr_vector);
    first = size/2;
    ivt = (INTR**)Malloc(sizeof(INTR*));
    create_interrupt_vector_array(intr_vector, ivt);
    my_dheap =  build_dynamic_array_heap_adt(first, amt, htype, itype, h1_compare, h1_process);
    for(int i = 0; i < first; i++)
        dheap_insert(my_dheap, ivt[i]);
    print_heap_ary(my_dheap);
    get_dheap_stats(my_dheap);
    puts("\nTRIGGER REALLOC 3 times:\n");
    for(int i = first; i < size; i++)
        dheap_insert(my_dheap, ivt[i]);
    puts("REPRINT\n");
    print_heap_ary(my_dheap);
    get_dheap_stats(my_dheap);
    destroy_dynamic_heap_array(my_dheap);
}


void h1_swap(void *arg1, void *arg2)
{
    INTR *parent, *child, *hold;
    
    parent = (INTR*)arg1;
    child = (INTR*)arg2;
    hold = (INTR*)malloc(sizeof(INTR));
    hold->description = parent->description;
    hold->priority = parent->priority;
    parent->description = child->description;
    parent->priority = child->priority;
    child->description = hold->description;
    child->priority = hold->priority;
}

void sample_ptrbased_heap(char *intr_vector)
{
    int max_size;
    PHEAP *my_pheap;
    INTR **ivt;//interreupt-vector table

    max_size = get_line_count(intr_vector);
    my_pheap =  create_pointer_based_heap(max_size, h1_compare, h1_process, h1_swap);
    ivt = (INTR**)Malloc(sizeof(INTR*));
    create_interrupt_vector_array(intr_vector, ivt);
    for(int i = 0;  i < max_size; i++)
        pointer_based_heap_insert(my_pheap, ivt[i]);
    puts("BREADTH FIRST TRAVERSAL");
    pointer_based_heap_breath_first_traversal(my_pheap);
    puts("GET MAX (ROOT)");
    pointer_based_heap_get_root(my_pheap);
    puts("NOW, BREADTH FIRST TRAVERSAL:\n");
    pointer_based_heap_breath_first_traversal(my_pheap);
    destroy_pointer_based_heap(my_pheap);
}

IPv4_ADDR *addr_32_bit(uint8_t ip_addr[], int prefix)
{
    IPv4_ADDR *ipv4_addr = (IPv4_ADDR*)malloc(sizeof(IPv4_ADDR));
    uint32_t addr_32 = 0;
    uint32_t subnet_mask = 0;
    uint32_t network_addr = 0;
 
    //CONVERT TO BINARY 32-BIT COMPUTER ADDRESS
    for(int i = 0; i < 4; i++)
        addr_32 = addr_32*256 + ip_addr[i];
    //OBTAIN SUBNET MASK FROM PREFIX LENGTH
    for(int i = 32 - prefix; i < 32; i++)
        subnet_mask = subnet_mask | (1 << i);
    //OBTAIN NETWORK ADDRESS
    network_addr = addr_32 & subnet_mask;
    ipv4_addr->ip_address = addr_32;
    ipv4_addr->subnet_mask = subnet_mask;
    ipv4_addr->network_address = network_addr;
    ipv4_addr->prefix_len = prefix;
    return ipv4_addr;
    
}

/* all these ranges would actuallly come a structute and these
 members would be set dynamically by the routing algorithms*/
//routing algorithm would populate the table first
int prefix24_hash(uint32_t network_address)
{
    //for this example, forwarding table /24, there are only
    //m = 5 possible outputing interfaces
    //we map ranges to hash into one of these possible values.
    int intf;
    
    if(network_address >= PREFIX_BK[0] && network_address <= PREFIX_BK[1])
        intf = 0;
    else if(network_address >= PREFIX_BK[2] && network_address <= PREFIX_BK[2])
        intf = 1;
    else if(network_address >= PREFIX_BK[4] && network_address <= PREFIX_BK[5])
        intf = 2;
    else if(network_address >= PREFIX_BK[6] && network_address <= PREFIX_BK[7])
        intf = 3;
    else if(network_address >= PREFIX_BK[8] && network_address <= PREFIX_BK[9])
        intf = 4;
    else
        intf = 0;
    
    return intf;
}

ROUTE_24** testing_fwd_table_p24(int intf_count)
{
    ROUTE_24 **table = (ROUTE_24**)calloc(5, sizeof(ROUTE_24*));
    ROUTE_24 *route;
    for(int i = 0; i < intf_count; i++)
    {
        route = (ROUTE_24*)malloc(sizeof(ROUTE_24));
        route->outgoing_intf = i;
        table[i] = route;
    }
    
    return table;
}

PAGE* create_test_page_table(int size)
{
    PAGE *table = (PAGE*)calloc(size, sizeof(PAGE));
    
    for(int i=0; i < size; i++)
    {
        if(i%2 == 0)
            table[i].valid = 1;
        else
            table[i].valid = 0;
        
        table[i].ppn = i + 3;
    }
    return table;
}

/*
 hex and strcat
 strtol ect. for concise represeentaiton... 32-bit addreseese etc.
 */
void hash_process(void *page_table_data, void *virtual_address)
{
    VIRTUAL_ADDRESS *va;
    PAGE *page_table_entry;
    
    page_table_entry = (PAGE*)page_table_data;
    va = (VIRTUAL_ADDRESS*)(virtual_address);
    printf("INDEX: %d\n", va->vpn);
    printf("VIRTUAL ADDRESS: %d|%d\n", va->vpn, va->offset);
    printf("PHYSICAL ADDRESS: %d|%d\n", page_table_entry->ppn , va->offset);
}

/*
In this small example, we consider the PAGE TABLE HOLDS which
holds ADDRESSES OF ALL VIRUTAL PAGES ON DISK, and we give an example
with a DIRECT 1:1 MAPPING, in which case FOR DIRECT ADDRESSING
key = index
 */
int hash_function(void *data)
{
    int direct_addressing_idx;
    VIRTUAL_ADDRESS *va;
    
    va = (VIRTUAL_ADDRESS*)data;
    direct_addressing_idx = va->vpn;
    printf("idx===%d\n", va->vpn);
    return direct_addressing_idx;
}

//sample array with virtual addresses
VIRTUAL_ADDRESS *create_test_va_array(int size)
{
    VIRTUAL_ADDRESS *va_ary = (VIRTUAL_ADDRESS*)calloc(size, sizeof(VIRTUAL_ADDRESS));
    for(int i=0; i < size; i++)
    {
        va_ary[i].vpn = i+5;
        va_ary[i].offset = (int)pow(2,i);
    }
    return va_ary;
}

//review here:https://people.cs.pitt.edu/~xianeizhang/notes/vm.html
void sample_array_ht(void)
{
    printf("EXAMPLE USING DIRECT ADDRESSING SIMULATION A PAGE TABLE!!!!\n");
    HASH_TABLE *my_ht;
    PAGE *my_table;
    VIRTUAL_ADDRESS *va_ary;
    PROBING_TYPE p_type;
    HASHING_FUNCTION_TYPE h_type;
    COLLISION_RESULTION c_type;
    
    my_table = create_test_page_table(10);//0-9
    va_ary = create_test_va_array(10);
    p_type = DIRECT_ADDRESSING;
    h_type = DIRECT;
    c_type = NONE;
    
    my_ht = create_hash_table(10, 0, hash_process, NULL, NULL, p_type, h_type, c_type);
    insert_ht(my_ht, &my_table[0]+5, va_ary[0].vpn);
    puts("SEARCH THE HASH TABLE:\n");
    if(search_ht(my_ht, NULL, va_ary[0].vpn))//VPN is my IDX into the Page Table
        printf("found!\n");
    void *data = retrieve_ht(my_ht, NULL, va_ary[0].vpn);
    PAGE *rd = (PAGE*)data;
    printf("PPN: %d, VALID_bit: %d\n",rd->ppn, rd->valid);
    hash_process(&my_table[5], &va_ary[0]);
    printf("addr: %p\n", &my_table[0]+5);
    if(delete_ht(my_ht, NULL, va_ary[0].vpn))
        printf("deleted entry!\n");
    free(va_ary);
    free(my_table);
    free_ht(my_ht);
}

int hash_ascii_to_int(char *name, int size)
{
    int ascii_sum = 0;
    
    for(char *p = name; *p; p++)
        ascii_sum += *p;
    ascii_sum += rand()%37;
    return ascii_sum%size;
}

//http://www.cse.psu.edu/~gxt29/teaching/underpl/Scott4eSupplement.pdf
//https://www.geeksforgeeks.org/symbol-table-compiler/
void sample_ht_array_with_collision_resol(char *symbol_table)
{
    printf("EXAMPLE USING OPEN ADDRESSING SIMULATION WITH SYMBOL TABLE!!!!\n\n");
    //you can see collions and linear resolutions....
    HASH_TABLE *sym_table;
    SYM *data_in, *data_out;
    FILE *fp;
    char buffer[MAX_LINE], *entry_name;
    long strLen;
    int size;
    
    size =  get_line_count(symbol_table);
    sym_table = create_hash_table(size, 0, NULL, NULL, NULL, OPEN_ADDRESSING, DIVISION, ARRAY);
    fp = Fopen(symbol_table, "r");
    while(fgets(buffer, MAX_LINE, fp))
    {
        puts(buffer);
        data_in = (SYM*)malloc(sizeof(SYM));
        entry_name = strtok(buffer, ":");
        strLen = strlen(entry_name)+1;
        data_in->name =  (char*)malloc(sizeof(char)*strLen);
        strncpy(data_in->name,entry_name, strLen);
        data_in->hash_key = hash_ascii_to_int(data_in->name, size);
        data_in->var_type = (int)strtol(strtok(NULL, ":"),(char**)NULL, 10);
        data_in->var_scope = (int)strtol(strtok(NULL, ":"),(char**)NULL, 10);
        insert_ht(sym_table, data_in, data_in->hash_key);
        puts("\n");
    }
    Fclose(fp);
    
    puts("\nSAMPLE RETRIVAL");
    data_out = retrieve_ht(sym_table, NULL, 2);
    printf("%s:%d\n", data_out->name, data_out->hash_key);
    free_ht(sym_table);
}

void sample_hashed_table_with_bucket(char *symbol_table, int bucket_size)
{
    printf("EXAMPLE WITH BUCKET RESOLUTION!!!!\n\n");
    HASH_TABLE *sym_table;
    SYM *data_in, *data_out;
    FILE *fp;
    char buffer[MAX_LINE], *entry_name;
    long strLen;
    int size;

    
    size =  get_line_count(symbol_table);
    sym_table = create_hash_table(size, bucket_size, NULL, NULL, NULL, OPEN_ADDRESSING, DIVISION, BUCKET);
    fp = Fopen(symbol_table, "r");
    while(fgets(buffer, MAX_LINE, fp))
    {
        puts(buffer);
        data_in = (SYM*)malloc(sizeof(SYM));
        entry_name = strtok(buffer, ":");
        strLen = strlen(entry_name)+1;
        data_in->name =  (char*)malloc(sizeof(char)*strLen);
        strncpy(data_in->name,entry_name, strLen);
        data_in->hash_key = hash_ascii_to_int(data_in->name, size);
        data_in->var_type = (int)strtol(strtok(NULL, ":"),(char**)NULL, 10);
        data_in->var_scope = (int)strtol(strtok(NULL, ":"),(char**)NULL, 10);
        insert_ht(sym_table, data_in, data_in->hash_key);
        puts("\n");
    }
    Fclose(fp);
    puts("\nSAMPLE RETRIVAL");
    data_out = retrieve_ht(sym_table, NULL, 2);
    printf("%s:%d\n", data_out->name, data_out->hash_key);
    free_ht(sym_table);

}

int chain_compare(void *a, void *b)
{
    EMPLOYEE *c, *d;
    
    c = (EMPLOYEE*)a;
    d = (EMPLOYEE*)b;

    if(c->EN > d->EN)
        return +1;
    else if(c->EN < d->EN)
        return -1;
    else
        return 0;
}

void sample_hashed_table_with_chaining(char *db_in)
{
    printf("EXAMPLE HASH TABLE WITH CHAINING RESOLUTION FOR DATABASE SIMULATION!!!!\n\n");
    HASH_TABLE *org_db;
    EMPLOYEE *e, *ary, *data_out;
    int size, strLen, i;
    char *name;
    FILE *fp;
    char buffer[MAX_LINE];
    
    size =  get_line_count(db_in);
    ary = (EMPLOYEE*)Malloc(size*sizeof(EMPLOYEE));
    org_db = create_hash_table(size, 0, NULL, chain_compare, NULL, OPEN_ADDRESSING, DIVISION, CHAINING);
    fp = Fopen(db_in, "r");
    i = 0;
    while(fgets(buffer, MAX_LINE, fp))
    {
        puts(buffer);
        e = (EMPLOYEE*)Malloc(sizeof(EMPLOYEE));
        name = strtok(buffer, ":");
        strLen = (int)strlen(name) + 1;
        e->first = (char*)Malloc(sizeof(char)*strLen);
        strncpy(e->first, name, strLen);
        e->EN = (int)strtol(strtok(NULL, ":"),(char**)NULL, 10);
        insert_ht(org_db, e, e->EN);
        ary[i] = *e; i++;
        puts("\n");

    }
    Fclose(fp);
    puts("NOW WE ARE GOING TO RETRIEVE EACH ONE OF THOSE:");
    for(int i = 0; i < size; i++)
    {
            printf("searching for: %s with %d\n", ary[i].first, ary[i].EN);
            data_out = (EMPLOYEE*)retrieve_ht(org_db, &ary[i], ary[i].EN);
            if(data_out)
                printf("%s:%d\n\n", data_out->first, data_out->EN);
            else
                printf("NOT FOUND!\n");
    }
    puts("LET'S DELETE SOMETHING:\n");
    printf("TARGET IS %s:%d\n", ary[1].first, ary[1].EN);
    if((EMPLOYEE*)delete_ht(org_db, &ary[1], ary[1].EN))
        printf("SUCESS\n");
    free_ht(org_db);
}

void set_sample_prefix_table(char *in)
{
    FILE *fp;
    char buffer[MAX_LINE];
    int i;
    
    fp = Fopen(in, "r");
    i = 0;
    while(fgets(buffer, MAX_LINE, fp))
    {
       PREFIX_BK[i] = string_to_binary_ip(buffer); i++;
        
    }
    Fclose(fp);
}

//mini forwarding table of 5 entries -- will hash all /24 addresses
//to some outgoing interface based on the set ranges populated by the routing algorithm(s) used.
void sample_ht_with_route_fwd_table(char *input_file)
{
    //ROUTING TABLE = AN ARRAY OF HASHTABLES
    set_sample_prefix_table(input_file);
    HASH_TABLE *ipv4_routing_table = create_hash_table(32, 0, NULL, NULL, NULL, DIRECT_ADDRESSING, DIRECT, NONE);
    HASH_TABLE *fwd_table_24 = create_hash_table(5, 0, NULL, NULL, prefix24_hash, DIRECT_ADDRESSING, DIRECT, NONE);
    insert_ht(ipv4_routing_table, fwd_table_24, 24-1);
    int intf_count = 5;
    ROUTE_24 **test_routing_table = testing_fwd_table_p24(5);
    //GENERATE HASH TABLE ENTRIES
    for(int i = 0; i < intf_count; i++)
        insert_ht(fwd_table_24, test_routing_table[i], i);
    //TEST FORWARDING
    uint8_t ary[] = {198,28,0,0};
    IPv4_ADDR *e1 = addr_32_bit(ary, 24);
    ROUTE_24 *data_out = (ROUTE_24*)retrieve_ht(fwd_table_24, NULL, e1->network_address);
    printf("outgoing interface is %d\n", data_out->outgoing_intf);
}

int compare2(void *arg1, void *arg2)
{
    PERSON *a, *b;
    a = (PERSON*)arg1;
    b = (PERSON*)arg2;
    if(a->age > b->age)
        return +1;
    else if (a->age < b->age)
        return -1;
    else
        return 0;
}


/*
let's use some keys easier to see:

tree looks like:

            (8)
    (1)             (26)
            (20)            (70)

*/
void sample_bst(char *in)
{
    BST *b;
    FILE *fp;
    PERSON *p;
    char buffer[MAX_LINE];
    char *first, *last;
    int n;
    bool found;

    //puts("PRINT BST!!!!\n");
    b = create_bst(compare2);
    fp = Fopen(in, "r");
    while(fgets(buffer, MAX_LINE, fp))
    {
        p = (PERSON*)Malloc(sizeof(PERSON));
        first = strtok(buffer, ":");
        n = (int)strlen(first)+1;
        p->first = (char*)Malloc(n*sizeof(char));
        strncpy(p->first, first, n);
        last = strtok(NULL, ":");
        n = (int)strlen(last)+1;
        p->last = (char*)Malloc(n*sizeof(char));
        strncpy(p->last, last, n);
        p->age = (int)strtol(strtok(NULL, ":"), (char**)NULL, 10);
        p->ssn = (int)strtol(strtok(NULL, ":"), (char**)NULL, 10);
        bst_insert(b, p);
    }
    puts("PRE ORDER!!!!");
    preorder_traversal(b->root, process);
    puts("POST ORDER!!!!");
    postorder_traversal(b->root, process);
    puts("IN ORDER!!!!");
    inorder_traversal(b->root, process);
    puts("BFT!!!");
    breath_first_traversal(b->root, process);
    puts("BF-SEACH!!!");
    
    found = breath_first_search(b, p);//use last one
    if(found == true)
        printf("yayyy we found %s!\n", p->first);
}

void sample_sorting_algorithms(void)
{
    printf("for very small lists, n in [10, 20].\n");
    int ary[] = {23, 78, 45, 32, 56};
    printf("original array: \n");
    for(int i = 0; i < 5; i++)
        printf("%d ", ary[i]);
    printf("\n");
    //=== COMMNET OUT ONE AT A TIME ====
    //insertion_sort(ary, 5);
    //shell_sort(ary, 4);
    //bubble_sort(ary, 4);
    //quick_sort(ary, 0, 4);
    //selection_sort(ary, 4);
    //heap_sort(ary, 4);
    //merge_sort(ary, 0, 4);
    printf("sorted array: \n");
    for(int i = 0; i < 5; i++)
        printf("%d ", ary[i]);
    printf("\n");
}
