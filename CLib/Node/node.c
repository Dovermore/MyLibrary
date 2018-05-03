/*****************************************************************************
 *Author          Calvin Zhuoqun Huang
 *Licence         MIT Licence
 *File Name       node.c
 *Space for Tab   YES
 *Tab size        4
 *Time Created    27 April 2018 (Friday)

 *Brief:
    General purpose node library
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "node.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define INVALID -1
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */


/* <FunctionPrototypes> */
/* –––––––––––––––––––– */
/* </FunctionPrototypes> */

struct node_s {
    int weight;

    int nitem;

    void *keys;
    size_t skey;

    void *data;
    size_t sdata;

    int nchild;
    Node **childs;
};

/* Construct and return a new node */
Node *new_node(int nitem, size_t skey, size_t sdata, int nchild) {
    assert(nitem >= 0 && skey > 0 && sdata > 0 && nchild >= 0);
    Node *node = (Node *)malloc(sizeof*node);
    assert(node != NULL);

    node->nitem  = nitem;
    node->skey   = skey;
    node->sdata  = sdata;
    node->weight = INVALID;
    node->nchild = nchild;

    node->keys = (void*)calloc(nitem * skey, skey);
    node->data = (void*)calloc(nitem * sdata, sdata);

    node->childs = (Node**)malloc(sizeof*node->childs);
    for (int i = 0; i < nchild; i++) {
        node->childs[i] = NULL;
    }
    assert(node->keys != NULL && node->data != NULL && node->childs != NULL);

    return node;
}

/* assign key to node */
void ak(Node *node, void *key, int ind) {
    assert(node != NULL);
    assert(ind < node->nitem);
    memcpy(((char*)(node->keys)) + ind*node->skey, key, node->skey);
}

/* assign data to node based on index */
void ad(Node *node, void *data, int ind) {
    assert(node != NULL);
    assert(ind < node->nitem);
    memcpy(((char*)(node->data)) + ind*node->sdata, data, node->sdata);
}

/* assign key and data to node based on index */
void ai(Node *node, void *key, void *data, int ind) {
    ak(node, key, ind);
    ad(node, data, ind);
}

/* assign child to a parent based on index */
void ac(Node *parent, Node *child, int ind) {
    assert(parent != NULL);
    assert(ind < parent->nchild);
    parent->childs[ind] = child;
}

/* update the weight of the node */
void aw(Node *node, int weight) {
    assert(node != NULL);
    node->weight = weight;
}

/* return pointer to value of the node's key of certain index */
void *ck(Node *node, int ind) {
    assert(node != NULL);
    assert(ind < node->nitem);
    return (void*)(((char*)node->keys) + ind*node->skey);
}

/* return pointer to value of the node's data of certain index */
void *cd(Node *node, int ind) {
    assert(node != NULL);
    assert(ind < node->nitem);
    return (void*)(((char*)node->data) + ind*node->sdata);
}

/* return value of the node's weight */
int cw(Node *node) {
    assert(node != NULL);
    return node->weight;
}

/* return node's child at ind location */
Node *cc(Node *node, int ind) {
    assert(node != NULL);
    assert(ind < node->nchild);
    return node->childs[ind];
}

/* free memory allocated node */
void free_node(Node *node) {
    assert(node != NULL);
    free(node->childs);
    free(node->keys);
    free(node->data);
    free(node);
}













