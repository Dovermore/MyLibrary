

/* Header guard */
#ifndef _POLY_BBST_H
#define _POLY_BBST_H


typedef struct tree_s Tree;


Tree *new_tree(size_t skey, size_t sdata, int (*cmp)(void*, void*));
void tree_insert(Tree *t, void *key, void *data);
void check_data(Tree* t, void *key);
void print_tree(Tree *t);


#endif