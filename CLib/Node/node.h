/*****************************************************************************
 *Author          Calvin Zhuoqun Huang
 *Licence         MIT Licence
 *File Name       node.h
 *Space for Tab   YES
 *Tab size        4
 *Time Created    27 April 2018 (Friday)

 *Brief:
    Header file of General purpose node library
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

//node.h file
#ifndef _node_h
#define _node_h
// 

typedef struct node_s Node;

/* Construct and return a new node */
Node *new_node(int nitem, size_t skey, size_t sdata, int nchild);

/* assign key to node based on index */
void ak(Node *node, void *key, int ind);

/* assign data to node based on index */
void ad(Node *node, void *data, int ind);

/* assign key and data to node based on index */
void ai(Node *node, void *key, void *data, int ind);

/* assign child to a parent based on index */
void ac(Node *parent, Node *child, int ind);

/* update the weight of the node */
void aw(Node *node, int weight);

/* return pointer to value of the node's key of certain index */
void *ck(Node *node, int ind);

/* return pointer to value of the node's data of certain index */
void *cd(Node *node, int ind);

/* return value of the node's weight */
int cw(Node *node);

/* return node's child at ind location */
Node *cc(Node *node, int ind);

/* free memory allocated node */
void free_node(Node *node);

#endif