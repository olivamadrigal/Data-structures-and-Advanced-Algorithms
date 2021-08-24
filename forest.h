#ifndef tree_h
#define tree_h
typedef struct tree TREE;

//SINGLE LEVEL TREES
//collection of trees (forest) with Find and Union algorithms.
typedef struct tree
{
    void *data;
    struct tree **branch_out;
    int (*compare)(void *a, void *b);
    int count;
    int processed;
    int *w_ary;
    int w;
    
}TREE;

typedef struct forest
{
    TREE **ary;
    int count;
    int (*compare)(void *a, void *b);
    bool mst_complete;
    int mst_cost;
    
}FOREST;

TREE *create_tree(int branch_max, int (*compare)(void *a, void *b), void *data_in)
{
    TREE *t = (TREE*)malloc(sizeof(TREE));
    t->count = 1;
    t->compare = compare;
    t->data = data_in;
    t->branch_out = (TREE**)calloc(branch_max, sizeof(TREE*));
    for(int i =0; i < branch_max; i++)
        t->branch_out[i] = NULL;
    t->w_ary = (int*)calloc(branch_max, sizeof(int));
    return t;
}

//search the forest to see if target is in one of its trees
TREE *find(TREE **ary, int size, void *endpoint)//"disjoint forest"
{
    for(int i = 0; i < size; i++)
    {
        //compare root
        if(ary[i]->compare(ary[i]->data, endpoint) == 0)
            return ary[i];
        //search it's branhces
        if(ary[i]->count > 1)
        {
            for(int j = 0; j < ary[i]->count-1; j++)
                if(ary[i]->compare(ary[i]->branch_out[j]->data, endpoint) == 0)
                    return ary[i];//ary[i]->branch_out[j];
        }
    }
    return NULL;
}

//return tree address where endpoint is located
TREE *find_loc(TREE **ary, int size, void *endpoint)//"disjoint forest"
{
    for(int i = 0; i < size; i++)
    {
        //compare root
        if(ary[i]->compare(ary[i]->data, endpoint) == 0)
            return ary[i];
        //search it's branhces
        if(ary[i]->count > 1)
        {
            for(int j = 0; j < ary[i]->count-1; j++)
                if(ary[i]->compare(ary[i]->branch_out[j]->data, endpoint) == 0)
                    return ary[i]->branch_out[j];
        }
    }
    return NULL;
}

//JOIN TWO TREES
void make_union(FOREST *f, void *pa, void *pb, int w)//"disjoint forest"
{
    TREE *ta, *tb;
    int i, j;
    
    f->mst_cost += w;
    //obtain pointer to location of pa and pb (a tree or subtree)
    ta = find_loc(f->ary, f->count, pa);
    tb = find_loc(f->ary, f->count, pb);
    //connect ta to tb and vice versa
    ta->branch_out[ta->count-1] = tb; //(ta)<------>(tb)
    //ta->branch_out[ta->count-1]->w = w;
    ta->w_ary[ta->count-1] = w;
    tb->branch_out[tb->count-1] = ta;
    // tb->branch_out[tb->count-1]->w = w;
    tb->w_ary[tb->count-1] = w;
    ta->count++;
    tb->count++;
    
    //delete tb from ary and shift elements
    if(f->count > 2)
    {
        for(i = 0; i < f->count; i++)
        {
            if(f->ary[i] == tb)
            {
                //shift array elements
                for(j = i; j < f->count - 1; j++)
                    f->ary[j] = f->ary[j+1];
                f->ary[j] = NULL;//set last to NULL
                    i = f->count;//break the loop
            }
        }
        f->count--;//shrink the forest.. until it has length 1 and so our MST is complete.
    }
    else
    {
        f->ary[1] = NULL;//set last to NULL
        f->count--;
        //MST COMPLETE, adding any more edges would create a cycle
        f->mst_complete = true;
    }
    
}

FOREST *create_forest(int count, int (*compare)(void *a, void *b))
{
    FOREST *f = (FOREST*)malloc(sizeof(FOREST));
    
    f->ary = (TREE**)calloc(count, sizeof(TREE*));//disjoing collection of trees
    for(int i = 0; i < count; i++)
        f->ary[i] = create_tree(count, compare, NULL);
    f->count = count;
    f->compare = compare;
    f->mst_complete = false;
    f->mst_cost = 0;
    return f;
}

#endif /* tree_h */
