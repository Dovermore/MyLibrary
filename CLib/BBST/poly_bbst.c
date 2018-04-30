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
// bool same_sign(int x, int y);
// int get_sign(int x);
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

// /*
//  * Iterative insert (key, data) pair to tree while keeping the tree balanced
//  */
// void tree_insert(Tree *t, void *key, void *data) {
//     assert(t != NULL);

//     int dir_pprev = INVALID,       /* Who is this pointer? */
//         dir_prev  = INVALID,       /* Where did it come from? */
//         dir_next  = INVALID,       /* <del>Why is he dying</del> 
//                                       Where is he going? */
//         dir_nnext = INVALID,
//         balance   = INVALID;

//     bool found    = FALSE;

//     Node *node    = new_tree_node(t),
//          *grandpa = NULL,
//          *parent  = NULL,
//          *next    = NULL,
//          *tmp     = t->root;

//     /* Prepare the new node */
//     ai(node, key, data, POS);
//     aw(node, CHILDNULL + 1);

//     /* Iterative insert to correct position */
//     while (tmp != NULL && found == FALSE) {
//         printf("update once +++++++++++++++++\n");
//         /* Increase the number of child of tmp by one */
//         aw(tmp, cw(tmp)+1);
//         dir_next = t->cmp(ck(node, POS), ck(tmp, POS)) > 0 ? RIGHT : LEFT;
//         next = cc(tmp, dir_next);

//         /* If found the slot for next node */
//         if (next == NULL) {
//             next = node;
//             ac(tmp, node, dir_next);
//             found  = TRUE;
//         }

//         if (parent != NULL && (balance=check_balance(parent)) == dir_prev) {
//             printf("not balanced\n");
//             print_tree(t);
//             /* zig-zag */
//             if (dir_prev != dir_next) {
//                 printf("zig-zag\n");
//                 rotate_nodes(tmp, dir_next, parent, dir_prev);
//                 print_tree(t);
//                 printf("end zig-zag\n");
//             }
//             tmp = rotate_nodes(parent, dir_prev, grandpa, dir_pprev);
//             if (grandpa == NULL) {
//                 t->root = tmp;
//             }
//             print_tree(t);
//         } else if (balance == !dir_prev) assert(0);
//         grandpa = parent;
//         parent = tmp;
//         tmp = cc(tmp, dir_next);
//         dir_pprev = dir_prev;
//         dir_prev  = dir_next;
//     }
//     // printf("node %p\n", node);
//     // printf("parent %p\n", parent);
//     if (!found && t->nele > 0) ac(parent, node, dir_prev);
//     else if (t->nele == 0) t->root = node;

//     /* update number of element in the tree */
//     t->nele++;
// }

/*
 * Iterative insert (key, data) pair to tree while keeping the tree balanced
 */
void tree_insert(Tree *t, void *key, void *data) {
    assert(t != NULL);

    /* update number of element in the tree */
    t->nele++;

    int log_nele  = 1, 
        nele      = t->nele;

    while (nele >>= 1) ++log_nele;

    int dirs[log_nele],
        dir       = INVALID,
        blce      = INVALID,
        diff      = 0,
        prev_dir  = 0,
        head      = 0;

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
        aw(tmp, get_nchild(tmp));
        prev_dir = std_diff(diff, 0);
        head--;
    }
    t->root = tmp;
}

void check_data(Tree* t, void *key) {
    Node *node = t->root;
    void *node_key = NULL;
    while (node != NULL) {
        node_key = ck(node, POS);
        if (t->cmp(key, node_key) > 0) {
            printf("key %d larger than %d\n", *(int*)key, *(int*)node_key);
            node = cc(node, RIGHT);
        } else if (t->cmp(key, node_key) == 0) {
            printf("Found key %d! with data %f\n", *(int*)key,
                                            *(float*)cd(node, POS));
            return;
        } else {
            printf("key %d less than %d\n", *(int*)key, *(int*)node_key);
            node = cc(node, LEFT);
        }
    }
    printf("didn't find the record\n"); 
}


void print_tree(Tree *t) {
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
        printf("Node: address %p key= %d, weight= %d\n",tmp, *(int*)ck(tmp, POS), cw(tmp));
    }
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