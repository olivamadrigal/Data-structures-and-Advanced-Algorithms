#ifndef bst_h
#define bst_h
#include "wrappers.h"
#include "queue.h"


typedef struct bnode
{
    void *data;
    struct bnode *right;
    struct bnode *left;
    
}BNODE;

typedef struct bst
{
    BNODE *root;
    int (*compare)(void *a, void *b);
    int count;
}BST;

BST* create_bst(int (*compare)(void* , void*));
bool bst_insert(BST *tree, void* data_in);
BNODE* _bst_insert(BST *tree, BNODE *root, BNODE *pnew);
//DEPTH FIRST TRAVERSALS
void preorder_traversal(BNODE *tree, void (*process)(void *data));
void inorder_traversal(BNODE *root, void (*process)(void *data));
void postorder_traversal(BNODE *root, void (*process)(void *data));
//BREATH FIRST SNAKE LIKE TRAVERSAL
void breath_first_traversal(BNODE *root, void (*process)(void *data));
BNODE* bst_delete(BST *tree, BNODE *root, void *key, bool *success);
bool breath_first_search(BST *root, void *key);
BST* destroy_bst(BST *tree);
void _delete_bst(BNODE *root);

BST* create_bst(int (*compare)(void* , void*))
{
    BST *tree = (BST*)malloc(sizeof(BST));
    tree->root = NULL;
    tree->count = 0;
    tree->compare= compare;
    return tree;
}

bool bst_insert(BST *tree, void* data_in)
{
    BNODE *pnew;
    if(!(pnew = (BNODE*)malloc(sizeof(BNODE))))
        return false;
    
    pnew->data = data_in;
    pnew->left = pnew->right = NULL;
    if(tree->count == 0)
        tree->root = pnew;
    else
       tree->root = _bst_insert(tree, tree->root, pnew);
    tree->count++;
    return true;
}

BNODE* _bst_insert(BST *tree, BNODE *root, BNODE *pnew)
{
    if(!root)
        return pnew;
    if(tree->compare(pnew->data, root->data) < 0 )
    {
        root->left = _bst_insert(tree, root->left, pnew);
        return root;
    }
    if(tree->compare(pnew->data, root->data) > 0)
    {
        root->right = _bst_insert(tree, root->right, pnew);
        return root;//not needed since we just unwind after we return from base.
    }
    return root;//CONTROL PURPOSES, NEVER HITS HERE.
}

//N L R
void preorder_traversal(BNODE *root, void (*process)(void *data))
{
    if(root)
    {
        process(root->data);
        preorder_traversal(root->left, process);
        preorder_traversal(root->right, process);
    }
}

//L N R
void inorder_traversal(BNODE *root, void (*process)(void *data))
{
    if(root)
    {
        inorder_traversal(root->left, process);
        process(root->data);
        inorder_traversal(root->right, process);
    }
}

// L R N
void postorder_traversal(BNODE *root, void (*process)(void *data))
{
    if(root)
    {
        postorder_traversal(root->left, process);
        postorder_traversal(root->right, process);
        process(root->data);
    }
}

//BREATH TRAVERSLA FOR A TREE
void breath_first_traversal(BNODE *root, void (*process)(void *data))
{
    QUEUE *q;
    BNODE *cur;
    
    q = create_queue_adt(process);
    enqueue_queue_adt(q, root);
    while(!is_queue_empty_queue_adt(q))
    {
        cur =  (BNODE*)deque_queue_adt(q);
        process(cur->data);
        if(cur->left)
            enqueue_queue_adt(q, cur->left);
        if(cur->right)
            enqueue_queue_adt(q, cur->right);
    }
}

bool breath_first_search(BST *tree, void *key)
{
    QUEUE *q;
    BNODE *cur;
    
    q = create_queue_adt(NULL);
    enqueue_queue_adt(q, tree->root);
    while(!is_queue_empty_queue_adt(q))
    {
        cur =  (BNODE*)deque_queue_adt(q);
        if(tree->compare(key, cur->data) == 0)
            return true;
        if(cur->left)
            enqueue_queue_adt(q, cur->left);
        if(cur->right)
            enqueue_queue_adt(q, cur->right);
    }
    return false;
}

BNODE* bst_delete(BST *tree, BNODE *root, void *key, bool *success)
{
    BNODE *del_node, *new_root, *exc, *hold;
    
    if(tree->compare(key, root->data) < 0)
        root->left = bst_delete(tree, root->left, key, success);
    else if(tree->compare(key, root->data) > 0)
        root->right = bst_delete(tree, root->right, key, success);
    else
    {//found it
        
        del_node = root;
        if(!root->left)
        {
            new_root = root->right;
            free(del_node);
            *success = true;
            return new_root;
        }
        else if(!root->right)
        {
            new_root = root->left;
            free(del_node);
            *success = true;
            return new_root;
        }
        else//both subtress are present
        {
            //find largest node on the left subtree
            exc = root->left;
            while(exc->right)
                exc = exc->right;
            hold = root->data;
            root->data = exc->data;
            exc->data = hold;
            root->left = bst_delete(tree, root->left, exc->data, success);
            
        }
    }
    
    return root;
}

BST* destroy_bst(BST *tree)
{
    if(tree->root)
        _delete_bst(tree->root);
    free(tree);
    return NULL;
}

void _delete_bst(BNODE *root)
{
    if(root)
    {
        _delete_bst(root->left);
        free(root->data);
        _delete_bst(root->right);
        free(root);
    }
}


#endif /* bst_h */

