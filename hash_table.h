//printf("when the universe of keys is small such that we can have a direct mapping of keys to slots.!!!\n");
//printf("in here, key k is stored in slot k, hence no real hash table needed... only array of size (U) = key space!!!\n");
//update chain, for LL we always do everything based on key and not ptr addresses.
//for tress etc we assign addresses etc to keep the given structure.

#ifndef hash_tables_h
#define hash_tables_h

typedef enum {DIRECT_ADDRESSING=1,OPEN_ADDRESSING=2,LINEAR_PROBING,QUADRATIC_PROBING,DOUBLE_HASHING} PROBING_TYPE;
typedef enum {DIRECT=1,DIVISION,MULTIPLICATION,UNIVERSAL} HASHING_FUNCTION_TYPE; //direct = key == idx, no hashing, several other methods
typedef enum {NONE=1,ARRAY=2,BUCKET=3,CHAINING=4} COLLISION_RESULTION;

typedef struct chain_node
{
    void *satellite;
    struct chain_node *next;
}CHAIN_NODE;


typedef struct chain
{
    CHAIN_NODE *primary_area;
    CHAIN_NODE *overflow_area;
    int (*compare)(void *arg1, void *arg2);
    int (*compare_keys)(void *arg1, int data_key);
    int count;
    
}CHAIN;

typedef struct hash_table
{
    void **ary;
    void ***bht;
    int  size;
    int  bucket_size;
    void (*process)(void *arg1, void *arg2);
    COLLISION_RESULTION c_type;
    int (*hash_function)(int m, int k, int i);
    int (*rt_hf)(uint32_t netmask);

}HASH_TABLE;


typedef int (*hash_func)(int m, int key);

HASHING_FUNCTION_TYPE universal_hashing_method(void);
//ADT DICTIONARY OPS
bool insert_ht(HASH_TABLE *ht, void *data_in, int idx);
bool search_ht(HASH_TABLE *ht, void *target, int key);
bool delete_ht(HASH_TABLE *ht, void *target, int key);
void *retrieve_ht(HASH_TABLE *ht, void *target, int key);
bool free_ht(HASH_TABLE *ht);
//DIRECT API
bool direct_addressing(HASH_TABLE *ht, void *data_in, int idx);
bool direct_search(HASH_TABLE *ht, int idx);
bool direct_delete(HASH_TABLE *ht, int idx);
void* direct_retrieve(HASH_TABLE *ht, int idx);
int division_method(int m, int key);
int multiplication_method(int m, int key);
int double_hashing(int m, int k, int i);
//h_k set_hash_function(HASHING_FUNCTION_TYPE h_type);
//ARRAY SPECIFIC
void set_hash_function(HASH_TABLE *ht, HASHING_FUNCTION_TYPE h_type, PROBING_TYPE p_type);
bool array_ht_insert(HASH_TABLE *ht, void *data_in, int key);
bool array_ht_delete(HASH_TABLE *ht, int key);//target = NULL for non-chain
bool array_ht_search(HASH_TABLE *ht, int key);//SEARCH == RETRIEVE hopefully O(1) IF NO COLLISIONS -- if we have collision, save idx....// worst-case based on hash function + probing
void *array_ht_retrieve(HASH_TABLE *ht, int key);//target = NULL for non-chain
//BUCKET
void create_table_with_buckets(HASH_TABLE *ht);
bool bucket_ht_insert(HASH_TABLE *ht, void *data_in, int key);
bool bucket_ht_search(HASH_TABLE *ht, void *data_in, int key);
bool bucket_ht_delete(HASH_TABLE *ht, int key);
void *bucket_ht_retrieve(HASH_TABLE *ht, int key);
bool free_ht_buckets(HASH_TABLE *ht);
//CHAIN
void create_chained_table(HASH_TABLE *ht, int (*compare)(void *arg1, void *arg2));
CHAIN* create_chain(int (*compare)(void *arg1, void *arg2));
bool chain_ht_insert(HASH_TABLE *ht, void *data_in, int key);
bool chain_ht_delete(HASH_TABLE *ht, void *target, int key);
bool chain_ht_search(HASH_TABLE *ht, void *target, int key);
void *chain_ht_retrieve(HASH_TABLE *ht, void *target, int key);
bool free_ht_chains(HASH_TABLE *ht);
void add_to_chain(CHAIN *list, CHAIN_NODE *pre, void *data_in);
bool delete_chain(CHAIN *list, CHAIN_NODE *pre, CHAIN_NODE *cur);
bool search_chain(CHAIN *list, CHAIN_NODE **pre, CHAIN_NODE **cur, void *target);
void *retrieve_chain(CHAIN *list, CHAIN_NODE **pre, CHAIN_NODE **cur, void *target);

#define MACHINE_WORD_SIZE 8
#define HASHING_METHODS 2
#define C1 5 // POSITIVE AUXILIARY CONSTANTS FOR QUAD PROBING
#define C2 9
#define HASH_TABLE_FULL printf("ERROR: HASH TABLE IS FULL!!\n");
#define BUCKET_FULL printf("ERROR: BUCKET IS FULL!!\n");

HASH_TABLE* create_hash_table(int m,
                              int bucket_size,
                              void (*process)(void *arg1, void *arg2),
                              int (*compare)(void *arg1, void *arg2),//chain specifc
                              int (*rt_hf)(uint32_t netmask),
                              PROBING_TYPE p_type,
                              HASHING_FUNCTION_TYPE h_type,
                              COLLISION_RESULTION c_type)
{
    HASH_TABLE *ht = (HASH_TABLE*)malloc(sizeof(HASH_TABLE));
    
    ht->size = m;
    ht->process = process;
    ht->c_type = c_type;
    ht->ary = (void**)calloc(m, sizeof(void*));

    if(c_type == NONE)//DIRECT 1:1
    {
        ht->bucket_size = 0;
        ht->bht = NULL;
        if(rt_hf)
            ht->rt_hf = rt_hf;
        else
            ht->rt_hf = NULL;
    }
    else if (c_type == ARRAY)//PROBING
    {
        set_hash_function(ht, h_type, p_type);
        
    }
    else if(c_type == BUCKET)//COLLISION RESOLVE WITH BUCKETS
    {
        ht->bucket_size = bucket_size;
        create_table_with_buckets(ht);
    }
    else//COLLISION RESOLVED WITH CHAINING
    {
        create_chained_table(ht, compare);
    }
    return ht;
}

CHAIN* create_chain(int (*compare)(void *arg1, void *arg2))
{
    CHAIN *list  = (CHAIN*)malloc(sizeof(CHAIN));
    list->overflow_area = NULL;
    list->primary_area = NULL;
    list->count = 0;
    list->compare = compare;
    
    return list;
}

//Just draw it out and then implementation is very easy.
void create_chained_table(HASH_TABLE *ht, int (*compare)(void *arg1, void *arg2))
{
    //ARRAY OF POINTERS TO VOID: TO A LINKED LIST OF APPLICATION DATA.
    
    for(int i = 0; i < ht->size; i++)
    {
        //each entry in the hash_table will have a pointer to void that
        //points to a linked list (chain)
        ht->ary[i] = create_chain(compare);
    }
    
}

void create_table_with_buckets(HASH_TABLE *ht)
{
    ht->bht = (void***)calloc(ht->size, sizeof(void**));
    
    for(int i = 0; i < ht->size; i++)
    {
        ht->bht[i] = (void**)calloc(ht->bucket_size, sizeof(void*));
    }
}

/********************** DIRECT HT APIS *********************/
//direct addressing
bool direct_addressing(HASH_TABLE *ht, void *data_in, int idx)
{
    ht->ary[idx] = data_in;
    
    return true;
}

void* direct_retrieve(HASH_TABLE *ht, int idx)
{
    void *data_out;
    
    if(ht->rt_hf != NULL)
        idx = ht->rt_hf(idx);
    data_out = ht->ary[idx];
    return data_out;

}

bool direct_search(HASH_TABLE *ht, int idx)
{
    
    if(ht->ary[idx])
        return true;
    else
        return false;
}

bool direct_delete(HASH_TABLE *ht, int idx)
{
    ht->ary[idx] = NULL;
    return true;
}

/*********** HASH FUNCTIONS ************/
/*######### NO PROBING ################# */
HASHING_FUNCTION_TYPE universal_hashing_method(void)
{
    hash_func H[2]; // in general, this would contain several hash functions
    HASHING_FUNCTION_TYPE h_type;
    
    H[0] = division_method;
    H[1] = multiplication_method;
    
    int i = rand() % 2 + 1;
    
    //we randomly select the hashing function.
    //this is randonmly set per run or reselected each time invoked BUT we must store
    //in the data which function was used to be able to retrieved later.
    h_type = H[i] == division_method ? DIVISION: MULTIPLICATION;
    return h_type;
}

int division_method(int m, int key)
{
    return m % key;
}

/* KNUTH SUGGEST A = 0.61... = (√5 - 1)/2 */
int multiplication_method(int m, int key)
{
    int p, min, max, s, A, f_A;
    
    p = m / 2;
    min = 1;
    max = (int)pow(2,MACHINE_WORD_SIZE) - 1;
    s = rand() % ((max - min)+1) + min;
    A = s / pow(2,MACHINE_WORD_SIZE);
    f_A = key*A - floor(key*A);
    
    return (int)floor(m*f_A);
}
/*######### WITH OPEN ADDRESSING################# */
int division_method_open_addressing(int m, int k, int i)
{
    printf("probe i %d\n", i);
    return ((k+i)%m);
}

int multiplication_method_open_addressing(int m, int k, int i)
{
    int p, min, max, s, A, f_A;
    
    p = m / 2;
    min = 1;
    max = (int)pow(2,MACHINE_WORD_SIZE) - 1;
    s = rand() % ((max - min)+1) + min;
    A = s / pow(2,MACHINE_WORD_SIZE);
    f_A = (k+i)*A - floor((k+i)*A);
    return (int)floor(m*f_A);
}
/*######### WITH LINEAR ADDRESSING ################# */
int division_method_linear_addressing(int m, int k, int i)
{
    return (k%m)+i;
}

int multiplication_method_linear_addressing(int m, int k, int i)
{
    int p, min, max, s, A, f_A;
    
    p = m / 2;
    min = 1;
    max = (int)pow(2,MACHINE_WORD_SIZE) - 1;
    s = rand() % ((max - min)+1) + min;
    A = s / pow(2,MACHINE_WORD_SIZE);
    f_A = (k)*A - floor((k)*A);
    return (int)(floor(m*f_A) + i);
}

/*######### WITH QUADRATIC ADDRESSING ################# */
int division_method_quadratic_addressing(int m, int k, int i)
{
    return ((k%m) + C1*i + C2*i*i) % m;
}

int multiplication_method_quadtratic_addressing(int m, int k, int i)
{
    int p, min, max, s, A, f_A;
    
    p = m / 2;
    min = 1;
    max = (int)pow(2,MACHINE_WORD_SIZE) - 1;
    s = rand() % ((max - min)+1) + min;
    A = s / pow(2,MACHINE_WORD_SIZE);
    f_A = (k)*A - floor((k)*A);
    
    return (((int)(floor(m*f_A))) + C1*i + C2*i*i) % m ;
}

/*######### WITH DOUBLE HASHING ################# */
int double_hashing(int m, int k, int i)
{
    int h1, h2;
    
    h1 = division_method(m, k);
    h2 = division_method(m, k);

    return (h1 + i*h2) %m;
}

void set_hash_function(HASH_TABLE *ht, HASHING_FUNCTION_TYPE h_type, PROBING_TYPE p_type)
{
    if(p_type == DOUBLE_HASHING)
    {
        ht->hash_function = double_hashing;
    }
    else
    {
        if (h_type == UNIVERSAL)
            h_type = universal_hashing_method();
        
        if(h_type == DIVISION)
        {
            switch(p_type)
            {
                case 2:
                    ht->hash_function = division_method_open_addressing;
                    break;
                case 3:
                    ht->hash_function = division_method_linear_addressing;
                     break;
                case 4:
                    ht->hash_function = division_method_quadratic_addressing;
                     break;
                default:
                    ;
            }//switch
        }
        else{
                switch(p_type)
                {
                    case 2:
                        ht->hash_function = multiplication_method_open_addressing;
                         break;
                    case 3:
                        ht->hash_function = multiplication_method_linear_addressing;
                         break;
                    case 4:
                        ht->hash_function = multiplication_method_quadtratic_addressing;
                         break;

               }//switch
       }
    }//if_else
    
}

/********************* ADT APIS ***************************/
bool insert_ht(HASH_TABLE *ht, void *data_in, int key)
{
    bool sucess;
    
    switch(ht->c_type)
    {
        case 1://NONE - no collisionS
            sucess = direct_addressing(ht, data_in, key);
            break;
        case 2://ARRAY - with probing
            sucess = array_ht_insert(ht, data_in, key); //probing is used
            break;
        case 3://BUCKET
            sucess = bucket_ht_insert(ht, data_in, key);
            break;
        case 4: //CHAIN
            sucess = chain_ht_insert(ht, data_in, key);
            break;
    }
    return sucess;
}

bool search_ht(HASH_TABLE *ht, void *data, int key)
{
    bool found;
    
    switch(ht->c_type)
    {
        case 1://NONE - no collisionS
            found = direct_search(ht, key);
            break;
        case 2://ARRAY - with probing
            found = array_ht_search(ht, key);
             break;
        case 3://BUCKET
            found = bucket_ht_insert(ht, data, key);
            break;
        case 4: //CHAIN
            found = chain_ht_search(ht, data, key);
            break;
    }
    return found;
}

bool delete_ht(HASH_TABLE *ht, void *target, int key)
{
    bool removed = false;
    
    switch(ht->c_type)
    {
        case 1://NONE - no collisionS
            removed = direct_delete(ht, key);
            break;
        case 2://ARRAY - with probing
            removed = array_ht_delete(ht, key);
             break;
        case 3://BUCKET
            removed = bucket_ht_delete(ht, key);
            break;
        case 4: //CHAIN
            removed = chain_ht_delete(ht, target, key);
            break;
    }
    return removed;

}

void *retrieve_ht(HASH_TABLE *ht, void *target, int key)
{
    void *data_out;
    
    switch(ht->c_type)
    {
        case 1://NONE - no collisionS
            data_out = direct_retrieve(ht, key);
            break;
        case 2://ARRAY - with probing
            data_out = array_ht_retrieve(ht, key);
            break;
        case 3://BUCKET
             data_out = bucket_ht_retrieve(ht, key);
            break;
        case 4: //CHAIN
            data_out = chain_ht_retrieve(ht, target, key);
            break;
    }
    return data_out;
    
}

//typedef enum {NONE=1,ARRAY=2,BUCKET=3,CHAINING=4} COLLISION_RESULTION;
bool free_ht(HASH_TABLE *ht)
{
    bool success = false;
    
    switch (ht->c_type) {
        case 1:
        case 2:
            free(ht->ary);
            free(ht);
            success = true;
            break;
        case 3:
            success = free_ht_buckets(ht);
            break;
        case 4:
            success = free_ht_chains(ht);
            break;
    }
    
    return success;
}


/********************** ARRAY HT APIS *********************/
/*
 ONLY THIS WILL USE PROBING SINCE FIXED has 1:1 mapping, and BUCKETS and CHAINING resolve collisions
 */
bool array_ht_insert(HASH_TABLE *ht, void *data_in, int key)
{
    int idx;
    bool found = false;
    for(int i = 0; i < ht->size; i++)
    {
        idx = ht->hash_function(ht->size, key, i);
        if(!ht->ary[idx])//found NULL SLOT
        {
            printf("inserted at %d\n", idx);
            ht->ary[idx] = data_in;
            i = ht->size;
            found = true;
        }
    }
    if(!found)
    {
        HASH_TABLE_FULL;
        exit(101);
    }
    return true;
}

//probe same sequence of slots
bool array_ht_search(HASH_TABLE *ht, int key)
{
    int  idx;
    bool found = false;
    
    for(int i = 0; i < ht->size; i++)
    {
        idx = ht->hash_function(ht->size, key, i);
        if(ht->ary[idx])
        {
            i = ht->size;
            found = true;
        }
    }
    return found;
}

bool array_ht_delete(HASH_TABLE *ht, int key)
{
    
    int  idx;
    bool found = false;
    
    for(int i = 0; i < ht->size; i++)
    {
        idx = ht->hash_function(ht->size, key, i);
        if(ht->ary[idx])
        {
            ht->ary[idx] = NULL;
            i = ht->size;
            found = true;
        }
    }
    return found;

}

void *array_ht_retrieve(HASH_TABLE *ht, int key)
{
    int  idx;
    void *data_out = NULL;
    
    for(int i = 0; i < ht->size; i++)
    {
        idx = ht->hash_function(ht->size, key, i);
        if(ht->ary[idx])
        {
            data_out = ht->ary[idx];
            i = ht->size;
        }
    }
    return data_out;
}

/********************** BUCKET HT APIS *********************/
bool bucket_ht_insert(HASH_TABLE *ht, void *data_in, int key)
{
    bool success = false;
    
    if(!ht->bht[key][0])
    {
        ht->bht[key][0] = data_in;
        printf("inserted at [%d][%d]\n", key, 0);
        success = true;
    }
    else
    {
        for(int i = 1; i < ht->bucket_size; i++)
        {
            if(!ht->bht[key][i])
            {
                ht->bht[key][i] = data_in;
                printf("inserted in bucket [%d][%d]\n", key, i);
                success = true;
                i = ht->bucket_size;
            }
            
        }//if this bucket is full, we have to probe next bucket... until exhausted all buckets.
    }
    return success;
}

bool bucket_ht_search(HASH_TABLE *ht, void *data_in, int key)
{
    bool success = false;
    
    if(ht->bht[key][0] == data_in)
        return true;
    else
    {   //search bucket
        for(int i = 1; i < ht->bucket_size; i++)
        {
            if(ht->bht[key][i] == data_in)
            {
                success = true;
                i = ht->bucket_size;
            }
            
        }
    }
    return success;
}

void *bucket_ht_retrieve(HASH_TABLE *ht, int key)
{
    void *data_out = NULL;
    
    if(ht->bht[key][0])
        return ht->bht[key][0];
    else
    {   //search bucket
        for(int i = 1; i < ht->bucket_size; i++)
        {
            if(ht->bht[key][i])
            {
                return ht->bht[key];
            }
            
        }
        
    }
    return data_out;

}

bool bucket_ht_delete(HASH_TABLE *ht, int key)
{
    bool success = false;
    
    if(ht->bht[key][0])
    {
        ht->bht[key][0] = NULL;
        return true;
    }
    else
    {
        for(int i = 1; i < ht->bucket_size; i++)
        {
            if(!ht->bht[key][i])
            {
                if(ht->bht[key][i])
                {
                    ht->bht[key][i] = NULL;
                    return true;
                }
            }
            
        }
        
    }
    return success;
}

bool free_ht_buckets(HASH_TABLE *ht)
{
    bool success = false;
    
    for(int i = 0; i < ht->size; i++)
    {
        free(ht->bht[i]);//free buckets
    }
    free(ht->bht);//free array of buckets
    ht->bht = NULL;
    free(ht);
    
    return success;
}


/********************** CHAIN HT APIS *********************/
bool chain_ht_insert(HASH_TABLE *ht, void *data_in, int key)
{
    CHAIN *c;
    CHAIN_NODE *pre, *cur, *pnew;
    bool success;
    
    key = key % ht->size;
    success = false;
    c = (CHAIN*)ht->ary[key];
    if(!c->primary_area)
    {
        printf("[%d]: inserted at primary area!\n", key);
        CHAIN_NODE *pnew = (CHAIN_NODE*)malloc(sizeof(CHAIN_NODE));
        pnew->satellite = data_in;
        c->primary_area = pnew;
        c->count++;
        success = true;
    }
    else
    {   //go to overflow
        printf("[%d]: inserted at overflow!\n", key);
        if(!search_chain(c, &pre, &cur, data_in))//no duplicates
        {
            add_to_chain(c, pre, data_in);
            success = true;
        }
        
    }
    return success;
}

bool chain_ht_search(HASH_TABLE *ht, void *data_in, int key)
{
    CHAIN *c;
    CHAIN_NODE *pre, *cur;
    bool found;
    
    found = false;
    c = (CHAIN*)ht->ary[key];
    
    if(c->primary_area)
    {
        if(c->compare(c->primary_area->satellite, data_in) == 0)
            found = true;
    }
    else //search the overflow area
        found = search_chain(c, &pre, &cur, data_in);
    return found;
}

void *chain_ht_retrieve(HASH_TABLE *ht, void *target, int key)
{
    CHAIN *c;
    CHAIN_NODE *pre, *cur;
    void *data_out = NULL;
    
    key = key % ht->size;
    c = (CHAIN*)ht->ary[key];
    if(c->primary_area)
    {
        if(c->compare(c->primary_area->satellite, target) == 0)
        {
            printf("found in primary area of [%d]!\n", key);
            return c->primary_area->satellite;
        }
        else
        {   //search this chain in the overflow
            data_out = retrieve_chain(c, &pre, &cur, target);
            if(data_out)
            {
                printf("found in overflow area of [%d]!\n", key);
            }
        }
    }
    return data_out;
}

bool chain_ht_delete(HASH_TABLE *ht, void *target, int key)
{
    CHAIN *c;
    CHAIN_NODE *pre, *cur;
    bool removed;
    
    removed = false;
    key = key % ht->size;
    c = (CHAIN*)ht->ary[key];
    if(c->primary_area)
    {
        if(c->compare(c->primary_area->satellite, target) == 0)
        {   //don't move from overflow here. just leave this NULL.
            //don't free anything here
            printf("deleting node from primary area [%d]\n", key);
            c->primary_area = NULL;
            c->count--;
            removed  = true;
        }
    }
    else
    {
        if(search_chain(c, &pre, &cur, target))
        {
            printf("deleting node from overflow area [%d]\n", key);
            delete_chain(c, pre, cur);
            c->count--;
            removed = true;
        }

    }
    
    return removed;
}

bool free_ht_chains(HASH_TABLE *ht)
{
    bool success = false;
    CHAIN *c;
    CHAIN_NODE *pre, *cur;
    
    for(int i = 0; i < ht->size; i++)
    {
        c = (CHAIN*)ht->ary[i];
        if(c->primary_area)
            free(c->primary_area);
        if(c->overflow_area)
        {
            cur = c->overflow_area;
            while(cur)
            {
                pre = cur;
                cur = cur->next;
                free(pre);
            }
        }
        success = true;
    }
    free(ht);
    return success;
}

void add_to_chain(CHAIN *list, CHAIN_NODE *pre, void *data_in)
{
    CHAIN_NODE *pnew = (CHAIN_NODE*)malloc(sizeof(CHAIN_NODE));
    pnew->satellite = data_in;
    
    if(!pre)//empty or start
    {
        pnew->next = list->overflow_area;
        list->overflow_area = pnew;

    }
    else//middle or end
    {
        pnew->next = pre->next;
        pre->next = pnew;
    }
    list->count++;
}

bool search_chain(CHAIN *list, CHAIN_NODE **pre, CHAIN_NODE **cur, void *target)
{
    bool found;
    
    found = false;
    *pre = NULL;
    if(list->overflow_area)
    {
        *cur = list->overflow_area;
        while(*cur && (list->compare(target, (*cur)->satellite) > 0))
        {
            *pre = *cur;
            *cur = (*cur)->next;
        }
        if(*cur && (list->compare(target, (*cur)->satellite) == 0))
            found = true;
    }
    return found;
}


//retrieve_chain(c, &pre, &cur, target);
/*
 Given chaining strucutre,
 once we index USING h(key) into the array, we must search the primary area
 or overflow chain. we identifiy what we are looking for based on the application key
 and not h(key).
 */
void *retrieve_chain(CHAIN *list, CHAIN_NODE **pre, CHAIN_NODE **cur, void *target)
{
    void *data_out = NULL;
    
    *pre = NULL;
    if(list->overflow_area)
    {
        *cur = list->overflow_area;
        while(*cur && (list->compare(target, (*cur)->satellite) > 0))
        {
            *pre = *cur;
            *cur = (*cur)->next;
        }
        if(*cur && (list->compare(target, (*cur)->satellite) == 0))
        {
            data_out = (*cur)->satellite;
        }
    }
    return data_out;
}

bool delete_chain(CHAIN *list, CHAIN_NODE *pre, CHAIN_NODE *cur)
{
    bool removed = false;
    
    //in overflow, we do delete nodes.
    if(!pre)//delete at front
    {
        list->overflow_area = list->overflow_area->next;
        if(list->count == 1)
        {
            list->overflow_area = NULL;
        }
        free(pre);
        removed = true;
    }
    else{//delete in the middle or the end
        
        pre->next = pre->next->next;
        free(cur);
        removed = true;
    }
    list->count--;
    return removed;
}
#endif /* hashing_tables_h */
