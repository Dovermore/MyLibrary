/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: poly_tree.h
 
 *This program is created for the purpose of:
        practice wrting tree structure in C.
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "poly_tree.h"


#define TRUE 1
#define FALSE 0

typedef struct node_s node_t;

struct node_s {
    void *data;
    node_t *left, *right;
};


typedef struct tree_s {
    node_t *root;
    int (*cmp)(void*, void*);
} tree_t;


tree_t *return_empty_tree(int (*cmpfunc)(void*, void*));
void *search_data(void *key, tree_t *tree);
tree_t *insert_data(void *key, tree_t *tree);
void retrieve_print_data(tree_t *tree, void (*printfunc)(void*));
void free_tree(tree_t *tree);
tree_t *insert_sorted_array_data(tree_t *tree, void**ary_data, size_t num_ele);
int is_bst(tree_t *tree);
double avg_depth(tree_t *tree);
int depth_deepest_node(tree_t *tree);
tree_t *delete_data(tree_t *tree, void *key);
int number_node(tree_t *tree);


/* Static functions for checking */
static tree_t *make_empty_tree(int (*cmpfunc)(void*, void*));
static node_t *recursive_seaching_tree(void *key, int (*cmp)(void*, void*), 
                                                            node_t *node);
static node_t *recursive_insert_data(void *key, int (*cmp)(void*, void*), 
                                                node_t *node);
static void recursive_retrieve_data(node_t *node, 
                                        void (*printfunc)(void*));
static void recursive_free_node(node_t *node);
static int recursive_depth_node(node_t *node, int depth, int *num);
static int recursive_check_subtree(node_t *node, int (*cmp)(void*, void*));
static void recursive_insert_array_data(node_t *node,
                                       void **ary_left, size_t num_left,
                                        void **ary_right, size_t num_right, 
                                        int (*cmp)(void*, void*))
static int recursive_deepest_node(node_t *node, int n);
static node_t *recursive_delete_key(void *key, node_t *node, int dir,
                                                int (*cmp)(void*, void*));
static node_t *recursive_rejoin_subtree(node_t *node, node_t *subtree,
                                                int (*cmp)(void*, void*));
/* Static functions for checking*/


static tree_t *make_empty_tree(int (*cmpfunc)(void*, void*)) {
    tree_t *tree = (tree_t*)malloc(sizeof(*tree));
    assert(tree != NULL);
    tree->cmp = cmpfunc;
    tree->root = NULL;
    return tree;
}

// return an empty tree structure
tree_t *return_empty_tree(int (*cmpfunc)(void*, void*)) {
    tree_t *tree = make_empty_tree(cmpfunc);
    assert(tree != NULL);
    return tree;
}


static node_t *recursive_seaching_tree(void *key, int (*cmp)(void*, void*), 
                                                            node_t *node) {
    int result;
    if (node == NULL) {
        return NULL;
    } else if ((result = cmp(key, node->data)) == 0) {
        return node;
    } else if (result < 0) {
        return recursive_seaching_tree(key, cmp, node->left);
    } else {
        return recursive_seaching_tree(key, cmp, node->right);
    }
}

// return node if key is found, else return NULL
void *search_data(void *key, tree_t *tree) {
    assert(tree != NULL);
    assert(key  != NULL);
    node_t *node = recursive_seaching_tree(key, tree->cmp, tree->root);
    return node != NULL ? node->data : NULL;
}


static node_t *recursive_insert_data(void *key, int (*cmp)(void*, void*), 
                                                node_t *node) {
    int result;
    if (node == NULL) {
        node = (node_t*)malloc(sizeof(node_t));
        assert(node != NULL);
        node->data = (void*)malloc(sizeof(void));
        assert(node->data != NULL);
        node->data = key;
        node->left = node->right = NULL;
        return node;
    } else if ((result = cmp(key, node->data)) <= 0) {
        node->left = recursive_insert_data(key, cmp, node->left);
    } else if (result > 0) {
        node->right = recursive_insert_data(key, cmp, node->right);
    }
    return node;
 }

// insert data and return the tree
tree_t *insert_data(void *key, tree_t *tree) {
    assert(tree != NULL);
    assert(key  != NULL);
    if ((tree->root = recursive_insert_data(key, tree->cmp, tree->root)) && 
                                        tree->root != NULL) {
        assert(tree->root != NULL);
    }
    return tree;
}


static void recursive_retrieve_data(node_t *node, 
                                        void (*printfunc)(void*)) {
    if (node != NULL) {
        recursive_retrieve_data(node->left, printfunc);
        printfunc(node->data);
        recursive_retrieve_data(node->right, printfunc);
    }
}


// based on print func, print all data by the default order
void retrieve_print_data(tree_t *tree, void (*printfunc)(void*)) {
    assert(tree != NULL);
    recursive_retrieve_data(tree->root, printfunc);
}


static void recursive_free_node(node_t *node) {
    if (node) {
        recursive_free_node(node->left);
        recursive_free_node(node->right);
        free(node);
        node->data = NULL;
    }
}


//free the tree
void free_tree(tree_t *tree) {
    assert(tree != NULL);
    recursive_free_node(tree->root);
    free(tree);
    tree->cmp = NULL;
}


static int recursive_depth_node(node_t *node, int depth, int *num) {
    int sum_depth = depth;
    if (node != NULL) {
        (*num)++;
        if (node->left) {
            sum_depth += recursive_depth_node(node->left, depth + 1, num);
        }
        if (node->right) {
            sum_depth += recursive_depth_node(node->right, depth + 1, num);
        }
        return sum_depth;
    } else return 0;
}


double avg_depth(tree_t *tree) {
    assert(tree != NULL);
    int num_pt = 0;
    double depth = (double)recursive_depth_node(tree->root, 1, &num_pt);
    return num_pt ? depth / num_pt : 0;
}


static int recursive_check_subtree(node_t *node, int (*cmp)(void*, void*)) {
    if (!node) return TRUE;
    if (node->left != NULL && cmp(node->left->data, node->data) > 0) {
        return FALSE;
    } else if (node->right != NULL && cmp(node->right->data, node->data) <= 0) {
        return FALSE;
    }
    return (recursive_check_subtree(node->left, cmp) && 
                        recursive_check_subtree(node->right, cmp));
}


int is_bst(tree_t *tree) {
    assert(tree != NULL);
    return recursive_check_subtree(tree->root, tree->cmp);
}


static void recursive_insert_array_data(node_t *node,
                                       void **ary_left, size_t num_left,
                                        void **ary_right, size_t num_right, 
                                        int (*cmp)(void*, void*)) {
    if (num_left) {
        node->left = recursive_insert_data(ary_left[num_left / 2],
                                                                    cmp, node->left);
        assert(node->left != NULL);
        recursive_insert_array_data(node->left, ary_left, num_left / 2,
                                    ary_left + (num_left / 2 + 1),
                                    num_left - num_left / 2 - 1,
                                    cmp);
    }
    if (num_right) {
        node->right = recursive_insert_data(ary_right[num_right / 2],
                                                                    cmp, node->right);
        assert(node->right != NULL);
        recursive_insert_array_data(node->right, ary_right, num_right / 2,
                                    ary_right + (num_right/2 + 1),
                                    num_right - num_right / 2 - 1, 
                                    cmp);
    }
}



// what if the tree is non_empty? how to evenly insert?
// And how about change the function to create and insert rather than just
// insert?
tree_t *insert_sorted_array_data(tree_t *tree, void **ary_data,
                                        size_t num_ele) {
    assert(tree != NULL);
    assert(ary_data != NULL);
    if (!num_ele) return tree;
    tree->root = recursive_insert_data((void*)(ary_data[num_ele / 2]),
                                                    tree->cmp, tree->root);
    recursive_insert_array_data(tree->root, ary_data, num_ele / 2,
                                ary_data + num_ele / 2 + 1,
                                num_ele - num_ele / 2 - 1, 
                                tree->cmp);
    return tree;
}


static int recursive_deepest_node(node_t *node, int n) {
    if (node == NULL) return n;
    n++;
    int left_depth = recursive_deepest_node(node->left, n);
    int right_depth = recursive_deepest_node(node->right, n);
    return left_depth > right_depth ? left_depth : right_depth;
}


int depth_deepest_node(tree_t *tree) {
    assert(tree != NULL);
    return recursive_deepest_node(tree->root, 0);
}


static node_t *recursive_delete_key(void *key, node_t *node, int dir,
                                                int (*cmp)(void*, void*)) {
    assert(node != NULL);
    if ((dir > 0 ? node->right : node->left) == NULL) {
        return NULL;
    }
    int result = cmp(key, (dir > 0 ? node->right->data : node->left->data));
    node_t *rem_node = NULL;
    if (result == 0) {
        rem_node = (dir > 0 ? node->right->left : node->left->right);
        if (dir > 0) node->right = node->right->right;
        else if (dir < 0) node->left = node->left->left;
        return rem_node;
    } else {
        return recursive_delete_key(key,
                                       (dir > 0 ? node->right : node->left),
                                                                result, cmp);
    }
}



static node_t *recursive_rejoin_subtree(node_t *node, node_t *subtree,
                                                int (*cmp)(void*, void*)) {
    int result = 0;
    if (node == NULL) return subtree;
    if ((result = cmp(subtree->data, node->data)) <= 0) {
        node->left = recursive_rejoin_subtree(node->left, subtree, cmp);
    } else if (cmp > 0) {
        node->right = recursive_rejoin_subtree(node->right, subtree, cmp);
    }
    return node;
}


tree_t *delete_data(tree_t *tree, void *key) {
    assert(tree != NULL);
    int result = 0;
    int (*funccmp)(void*, void*) = tree->cmp;
    node_t *rem_node = NULL;
    if (tree->root == NULL) {
        return tree;
    }
    if ((result = funccmp(key, tree->root->data)) == 0) {
        rem_node = tree->root->right;
        tree->root = tree->root->left;
        if (rem_node != NULL) {
            recursive_rejoin_subtree(tree->root, rem_node, funccmp);
        }
        return tree;
    }
    rem_node = recursive_delete_key(key, tree->root, result, funccmp);
    if (rem_node != NULL) {
        tree->root = recursive_rejoin_subtree(tree->root, rem_node, funccmp);
    }
    return tree;
}


int number_node(tree_t *tree) {
    assert(tree != NULL);
    int num_pt = 0;
    recursive_depth_node(tree->root, 1, &num_pt);
    return num_pt;
}