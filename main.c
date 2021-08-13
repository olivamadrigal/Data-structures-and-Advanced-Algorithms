#include "dynamic_arrays.h"
#include "arrays.h"
#include "linked_list.h"
#include "ht_linked_list.h"
#include "c_linked_list.h"
#include "d_linked_list.h"
#include "queue.h"
#include "stack.h"
/***** DATA STRUCTURES REVIEW USING ABSTRACT DATA TYPES with sample application data structure *****/

/*** for simplicity: all functions prototypes and source code in .h lib file as opposed to
function prototypes in .h file and functions definitions in .c. Input file names taken from Turkish dram series Sen anlat karadeniz.***/

typedef struct person
{
    char *first;
    char *last;
    int ssn;
    int age;
}PERSON;

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

#define LINKED_LIST_INPUT "input_data1.in"
#define DATA_INPUT2 "input_data2.in"
#define MAX_LINE 80
#define FLUSH while(getchar() != '\n')

int main(int argc, const char * argv[])
{
    //FIRST HALF CONSISTS OF LINEAR LISTS arrays and pointer-based.
    /*
    //sample data for realloc use
    int size = get_line_count(DATA_INPUT2);
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
        ary[idx]->first = (char*)malloc(sizeof(char)*len);
        strncpy(ary[idx]->first, token, len);
        token = strtok(NULL, ":");
        len = strlen(token) + 1;
        ary[idx]->last = (char*)malloc(sizeof(char)*len);
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
    target = (PERSON*)malloc(sizeof(PERSON));
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
    target = (PERSON*)malloc(sizeof(PERSON));
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
   
   
    target = (PERSON*)malloc(sizeof(PERSON));
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
    target = (PERSON*)malloc(sizeof(PERSON));
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
    target = (PERSON*)malloc(sizeof(PERSON));
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
    target = (PERSON*)malloc(sizeof(PERSON));
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

