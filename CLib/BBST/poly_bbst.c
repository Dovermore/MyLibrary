/*****************************************************************************
 *Author          Calvin Zhuoqun Huang ()
 *Licence         None
 *File Name       poly_bbst.c
 *Space for Tab   YES
 *Tab size        4

 *Brief:
    Polymophical Balance binary search tree
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "../Node/node.h"
#include "poly_bbst.h"

/* <MacroDefs> */
#define TRUE       1
#define FALSE      0
#define INVALID   -1

#define POS        0
#define NITEM      1
#define NCHILD     2

#define LEFT       0
#define RIGHT      1

#define CHILDNULL -1

#define MAXITER    1000

#define CLEAF      0
#define CLEFT      1
#define CRIGHT     2
#define CLEFTRIGHT 3
#define CRIGHTLEFT 4
/* </MacroDefs> */


/* <TypeDefs> */
struct tree_s {
    size_t skey,
           sdata;
    int nele;
    Node *root;
    int (*cmp)(void*, void*);
};
/* </TypeDefs> */


/* <FunctionPrototypes> */
static Node *new_tree_node(Tree *tree);
static Node *rotate_nodes(Node *node, int dir);
static int get_nchild(Node *node);
static int get_diff(Node *node);
static int std_diff(int n, int thresh);
static Node *retrieve_node(Tree *t, void *key)
static void recursive_retrieve_node(Node *node, Node *ary[], int *ind);
/*
 * Replace the keys and values of node a with node b's
 * but not childs
 */
static void replace_node(Node *a, Node *b);

/*
 * Helper function for returning the case integer in deleting nodes
 */
static int check_case(Node *node)
/* </FunctionPrototypes> */


Tree *new_tree(size_t skey, size_t sdata, int (*cmp)(void*, void*)) {
    Tree *t = (Tree*)malloc(sizeof*t);
    assert(t != NULL);
    t->skey  = skey;
    t->sdata = sdata;
    t->nele  = 0;
    t->root  = NULL;
    t->cmp = cmp;
    return t;
}


void free_tree(Tree *t) {
    assert(t != NULL);
    recursive_free_node(t->root);
    free(t);
}

static void recursive_free_node(Node *node) {
    if (node == NULL) return;
    recursive_free_node(cc(node, LEFT));
    recursive_free_node(cc(node, RIGHT));
    free_node(node);
}


void tree_insert(Tree *t, void *key, void *data) {
    assert(t != NULL);

    /* update number of element in the tree */
    t->nele++;

    int log_nele  = 0, 
        nele      = t->nele;
    while (nele >>= 1) ++log_nele;
    log_nele += 2;

    int dirs[log_nele],
        dir       = INVALID,
        blce      = INVALID,
        diff      = 0,
        prev_dir  = 0,
        head      = 0,
        nchild    = CHILDNULL;

    bool found    = FALSE;

    Node *stack[log_nele],
         *node    = new_tree_node(t),
         *parent  = NULL,
         *tmp     = t->root;

    /* Prepare the new node */
    ai(node, key, data, POS);
    aw(node, CHILDNULL + 1);

    /* Add the root to heap */
    if (t->root != NULL) {
        tmp = t->root;
    } else {
        t->root = node;
        return;
    }
    /* Iterative insert to correct position */
    while (tmp != NULL && found == FALSE) {
        dir = t->cmp(ck(node, POS), ck(tmp, POS)) > 0 ? RIGHT : LEFT;
        stack[head] = tmp;
        dirs[head]  = dir;
        head++;
        parent = tmp;
        tmp = cc(tmp, dir);
    }
    tmp = node;
    ac(parent, node, dir);
    /* now update the height of the tree and balance the tree */
    while (head) {
        ac(stack[head - 1], tmp, dirs[head - 1]);
        tmp = stack[head - 1];
        diff = get_diff(tmp);
        blce = std_diff(diff, 1);
        if (blce != INVALID) {
            if (prev_dir != blce) {
                ac(tmp, rotate_nodes(cc(tmp, blce), prev_dir), blce);
            }
            tmp = rotate_nodes(tmp, blce);
        }
        if ((nchild = get_nchild(tmp)) == cw(tmp)) {
            // The child is not updated, So we can break here
            break;
        }
        aw(tmp, nchild);
        prev_dir = std_diff(diff, 0);
        head--;
    }
    t->root = tmp;
}



void *delete_key(Tree *t, void *key) {
    assert(t != NULL);

    int log_nele  = 0, 
        nele      = t->nele;
    while (nele >>= 1) ++log_nele;
    log_nele += 2;

    int dirs[log_nele],
        dir       = INVALID,
        blce      = INVALID,
        diff      = 0,
        prev_dir  = 0,
        head      = 0,
        nchild    = CHILDNULL;

    bool found    = FALSE;

    Node *stack[log_nele],
         *parent  = NULL,
         *tmp     = t->root;

    void *data    = NULL;

    /* Iterative find position of the element with such key */
    while (tmp != NULL) {
        if ((dir = t->cmp(ck(node, POS), ck(tmp, POS))) > 0) {
            dir = RIGHT;
        } else if (dir < 0) {
            dir = LEFT;
        } else {
            // found the element
            break;
        }
        // record the path
        stack[head] = tmp;
        dirs[head]  = dir;
        head++;
        parent = tmp;
        tmp = cc(tmp, dir);
    }
    // Cant find such key
    if (tmp == NULL) return NULL;

    /* now starts the cycle of replace and delete */
    while (TRUE) {

        stack[head] = tmp;
        // These in the case
        // dirs[head]  = dir;
        // head++;      

        switch(check_case(tmp)) {
        
            case CLEAF:
                if (t->nele == 1) {
                    t->root = NULL;
                    return cd(tmp, POS);
                    // free_node(tmp); since the data is stored in node
                }
                
                break; /* optional */
            
            case CLEFT:
                /* statement(s) */
                break; /* optional */

            case CRIGHT:
                /* statement(s) */
                break; /* optional */

            case CLEFTRIGHT:
                /* statement(s) */
                break; /* optional */

            case CRIGHTLEFT:
                /* statement(s) */
                break; /* optional */
          
            default:
                printf("ERROR! case goes to bottom\n");
                /* statement(s) */
        }

    }



    /* now update the height of the tree and balance the tree */
    while (head) {
        ac(stack[head - 1], tmp, dirs[head - 1]);
        tmp = stack[head - 1];
        diff = get_diff(tmp);
        blce = std_diff(diff, 1);
        if (blce != INVALID) {
            if (prev_dir != blce) {
                ac(tmp, rotate_nodes(cc(tmp, blce), prev_dir), blce);
            }
            tmp = rotate_nodes(tmp, blce);
        }
        if ((nchild = get_nchild(tmp)) == cw(tmp)) {
            // The child is not updated, So we can break here
            break;
        }
        aw(tmp, nchild);
        prev_dir = std_diff(diff, 0);
        head--;
    }
    t->root = tmp;
}


void *retrieve_data(Tree *t, void *key) {
    assert(t != NULL);
    assert(key != NULL);
    return cd(retrieve_node(t, key), POS);
}


int retrieve_tree(Tree *t, void *key, void *data) {
    static int head;
    static Node *stack[MAXITER];
    static Node *tmp = NULL;
    
    if (key == NULL || data == NULL) {
        head = 0;
        if (t->root != NULL) {
            recursive_retrieve_node(t->root, stack, &head);
        }
        return TRUE;
    }
    if (head != 0) {
        tmp = stack[head - 1];
        head--;
        memcpy((char*)key, ck(tmp, POS), t->skey);
        memcpy((char*)data, cd(tmp, POS), t->sdata);
        return TRUE;
    }
    return INVALID;
}

int tree_nele(Tree *t) {
    assert(t!= NULL);
    return t->nele;
}



/* =========================Static functions======================== */

static void recursive_retrieve_node(Node *node, Node *ary[], int *ind) {
    if (node == NULL) {
        return;
    }
    recursive_retrieve_node(cc(node, RIGHT), ary, ind);
    ary[*ind] = node;
    (*ind)++;
    recursive_retrieve_node(cc(node, LEFT), ary, ind);
}


static Node *retrieve_node(Tree *t, void *key) {
    Node *node = t->root;
    void *node_key = NULL;
    while (node != NULL) {
        node_key = ck(node, POS);
        if (t->cmp(key, node_key) > 0) {
            node = cc(node, RIGHT);
        } else if (t->cmp(key, node_key) == 0) {
            return node;
        } else {
            node = cc(node, LEFT);
        }
    }
    return NULL;  
}


static Node *new_tree_node(Tree *t) {
    Node *node = new_node(NITEM, t->skey, t->sdata, NCHILD);
    assert(node != NULL);
    return node;
}

static Node *rotate_nodes(Node *node, int dir) {
    assert(node != NULL);
    Node *child = cc(node, dir);
    assert(child != NULL);
    ac(node, cc(child, !dir), dir);
    ac(child, node, !dir);
    /* Update the previous parent first, since it's now a child (The new 
       parent's data is dependent on this) */
    aw(node, get_nchild(node));
    /* Then update the previous child now parent */
    aw(child, get_nchild(child));
    return child;
}

static int get_nchild(Node *node) {
    Node *leftc = cc(node, LEFT),
         *rightc = cc(node, RIGHT);
    int num_leftc = leftc != NULL ? cw(leftc) : CHILDNULL,
        num_rightc = rightc != NULL ? cw(rightc) : CHILDNULL; 
    return (num_leftc > num_rightc ? num_leftc : num_rightc) + 1;
}

static int get_diff(Node *node) {
    Node *leftc = cc(node, LEFT),
         *rightc = cc(node, RIGHT);
    int num_leftc = leftc != NULL ? cw(leftc) : CHILDNULL,
        num_rightc = rightc != NULL ? cw(rightc) : CHILDNULL;
    return num_leftc - num_rightc;
}


static int std_diff(int n, int thresh) {
    if (n > thresh) return LEFT;
    else if (n < -thresh) return RIGHT;
    else return INVALID;
}


static void replace_node(Node *a, Node *b) {
    ak(a, ck(b, POS), POS);
    ad(a, cd(b, POS), POS);
}

static int check_case(Node *node) {
    Node *left   = cc(node, LEFT),
         *right  = cc(node, RIGHT),
         *lright = NULL,
         *rleft  = NULL;

    int nleft    = CHILDNULL,
        nright   = CHILDNULL,
        diff     = get_diff(node);

    if (left == NULL && right == NULL) {
        return CLEAF;
    } else if (left == NULL) {
        return CRIGHT;
    } else if (RIGHT == NULL) {
        return CLEFT;
    } else {
        lright = cc(left, RIGHT);
        rleft  = cc(right, LEFT);
        if (lright != NULL) {
            return CLEFTRIGHT;
        } else if (rleft != NULL) {
            return CRIGHTLEFT
        } else {
            return CLEFT;
        }
    }
}



void test_tree(Tree *t) {
    int start = 0, end = 0, num = t->nele+1;
    Node *queue[num],
         *tmp = NULL;
    if (t->root != NULL) {
        queue[end] = t->root;
        end = (end + 1)%(num + 1);
    }
    while (start != end) {
        tmp = queue[start];
        start = (start + 1)%num;
        if (cc(tmp, LEFT) != NULL) {
            queue[end] = cc(tmp, LEFT);
            end = (end + 1)%num;
        }
        if (cc(tmp, RIGHT) != NULL) {
            queue[end] = cc(tmp, RIGHT);
            end = (end + 1)%num;
        }
        printf("------------------------------------------------\n");
        printf("Node: address %p key= %d, weight= %d\n",tmp,
                                                *(int*)ck(tmp, POS), cw(tmp));
        printf("      Left child %p, Right child %p\n", cc(tmp, LEFT),
                                                        cc(tmp, RIGHT));
    }
}