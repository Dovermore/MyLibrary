/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: linked_list.h
 
 *This program is created for the purpose of:
    to practice using linked structure of list
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "poly_sorted_list.h"

// #define DEBUG 0

// #if DEBUG
// #define DUMPPT(x) printf("line %3d: %s = %p\n", __LINE__, #x, x)
// #else
// #define DUMPPT(x)
// #endif


#define TRUE 1
#define FALSE 0

typedef struct node_s node_t;

struct node_s {
    void *data;
    node_t *next;
};

typedef struct head_s {
    node_t *first;
    int num_ele;
    int (*cmp)(void*, void*);
    void (*fprint)(void*);
} head_t;




#ifndef _poly_sorted_list_h
#define _poly_sorted_list_h 
/* prototypes below */
head_t *make_new_head(int (*cmpfunc)(void*, void*),
                                            void (*func_print)(void*));
head_t *insert_key(void *key, head_t *head);
void print_whole_head(head_t *head);
void free_head(head_t *head);
head_t *delete_first(head_t *head);
head_t *delete_last(head_t *head);
/* prototypes above */
#endif




// void print_intf(void *n1);

static head_t *create_empty_list(int (*cmpfunc)(void*, void*),
                                                void (*func_print)(void*)) {
    head_t *head = (head_t*)malloc(sizeof(head_t));
    assert(head != NULL);
    head->first = NULL;
    head->cmp = cmpfunc;
    head->fprint = func_print;
    head->num_ele = 0;
    return head;
}


head_t *make_new_head(int (*cmpfunc)(void*, void*),
                                            void (*func_print)(void*)) {
    head_t *head = create_empty_list(cmpfunc, func_print);
    assert(head != NULL);
    return head;
}



static node_t *create_new_node(void *key) {
    node_t *new = (node_t*)malloc(sizeof(node_t));
    assert(new != NULL);
    new->data = key;
    new->next = NULL;
    return new;
}



static node_t *linear_insert_key(void *key, node_t *node,
                                                int (*cmp)(void*, void*)) {
    int cur_cmp;
    node_t *new = NULL;
    if (node != NULL) {
        cur_cmp = cmp(key, node->data);
        if (cur_cmp >= 0) {
            if (DEBUG) printf("Less than this node\n"); // test line
            new = create_new_node(key);
            new->next = node;
            return new;
        } else {
            if (DEBUG) printf("cmp result is %d\n", cur_cmp); // test line
            if (DEBUG) printf("Larger or equal to this node\n"); // test line
            node->next = linear_insert_key(key, node->next, cmp);
            return node;
        }
    } else {
        if (DEBUG) printf("Find NULL node\n"); //test line
        return create_new_node(key);
    }
}


head_t *insert_key(void *key, head_t *head) {
    assert(head != NULL);
    if (DEBUG) printf("\nStart Inserting\n"); //test line
    head->first = linear_insert_key(key, head->first, head->cmp);
    head->num_ele++;
    assert(head->first != NULL);
    return head;
}


static void *binary_search_nodes(void *key, node_t *left_node,
                                        node_t *right_node, int num, 
                                                int (*cmp)(void*, void*)) {
    node_t *mid_node = left_node;
    int i = 0, result = 0;
    for (i = 0; i < num / 2; i++) {
        mid_node = mid_node->next;
    }
    if (!num) return NULL;
    if ((result = cmp(key, mid_node->data)) == 0) {
        return mid_node->data;
    } else if (result < 0) {
        return binary_search_nodes(key, mid_node->next, right_node,
                                                    num - 1 - num / 2, cmp);
    } else {
        return binary_search_nodes(key, left_node, mid_node, num / 2, cmp);
    }
}



void *search_key(void *key, head_t *head) {
    assert(head != NULL);
    return binary_search_nodes(key, head->first, NULL,
                                                head->num_ele, head->cmp);
}





static int print_node_data(node_t *node, void (*func_print)(void*)) {
    if (node != NULL) {
        func_print(node->data);
        return TRUE;
    }
    return FALSE;
}


void print_whole_head(head_t *head) {
    assert(head != NULL);
    node_t *cur_node = head->first;
    printf("Totally %d elements were added\n---\n", head->num_ele);
    while (print_node_data(cur_node, head->fprint)) {
        cur_node = cur_node->next;
    }
}


static void recursive_free_del_node(node_t *node) {
    if (DEBUG) {
        printf("freeing\n");
        print_node_data(node, print_intf);
    }   
    if (node != NULL) {
        recursive_free_del_node(node->next);
        free(node->data);
        node->data = NULL;
        free(node);
        node = NULL;
    }
}

head_t *delete_first(head_t *head) {
    assert(head != NULL);
    assert(head->first != NULL);
    node_t *temp = head->first->next;
    free(head->first->data);
    free(head->first);
    head->first->data = NULL;
    head->first = temp;
    head->num_ele--;
    return head;
}


head_t *delete_last(head_t *head) {
    assert(head != NULL);
    assert(head->first != NULL);
    node_t *cur_node = head->first;
    if (cur_node->next == NULL) {
        delete_first(head);
    } else {
        while (cur_node->next->next != NULL) {
            cur_node = cur_node->next;
        }
        recursive_free_del_node(cur_node->next);
        cur_node->next = NULL;
        assert(cur_node->next == NULL);
    }
    head->num_ele--;
    return head;
}



void free_head(head_t *head) {
    assert(head != NULL);
    recursive_free_del_node(head->first);
    free(head);
    head->cmp = NULL;
    head->fprint = NULL;
    head = NULL;
}

/*
int int_desc_cmp(void* n1, void* n2) {
    int *x = (int*) n1, *y = (int*) n2;
    return *x - *y;
}


int int_asc_cmp(void* n1, void* n2) {
    int *x = (int*) n1, *y = (int*) n2;
    return *y - *x;
}

void print_intf(void *n1) {
    int *x = (int*) n1;
    printf("%d is the data stored!\n", *x);
}


void print_mystructf(void *n1) {
    my_struct_t *c = (my_struct_t*)n1;
    printf("a is %d, b is %d\n", c->a, c->b);
}


int my_struct_cmp(void *n1, void *n2) {
    my_struct_t *c1 = (my_struct_t*)n1;
    my_struct_t *c2 = (my_struct_t*)n2;
    return c1->a - c2->a;
}


int
main(int argc, char *argv[]) {
    srand(time(NULL));
    int num_iter = 1, i = 0, *num = (int*)malloc(sizeof(int)), j = 0;
    my_struct_t *mstrt = (my_struct_t*)malloc(sizeof(my_struct_t)),
                fdstrt = {1001010, 123123},
                *result_fd = NULL;
    head_t *new_head = NULL;
    if (argc > 1) {
        num_iter = atoi(argv[1]);
    }
    for (i = 0; i < num_iter; i++) {
        printf(SEPARATION);
        printf("%d times: scaffolding\n", i+1);
        printf(SEPARATION);
        new_head = make_new_head(my_struct_cmp, print_mystructf);
        assert(new_head != NULL);
        for (j = 0; j < 10; j++) {
            *num = (rand() - 1000000000) / 100000000;
            mstrt->a = *num;
            mstrt->b = 123123;
            if (DEBUG) printf("*num is %d\n", *num); //test line
            insert_key(mstrt, new_head);
            mstrt = (my_struct_t*)malloc(sizeof(my_struct_t));
        }
        printf("Printing in ascending order\n");
        printf("---\n");
        print_whole_head(new_head);
        printf(SEPARATION);
        printf("Finding struct:\n");
        print_mystructf(&fdstrt);
        result_fd = search_key(&fdstrt, new_head);
        printf("The result is:\n");
        !result_fd ? printf("No result found\n") : print_mystructf(result_fd);
        printf(SEPARATION);
        printf("Deleting last item\n---\n");
        delete_last(new_head);
        print_whole_head(new_head);
        printf(SEPARATION);
        printf("Deleting first item\n---\n");
        delete_first(new_head);
        print_whole_head(new_head);
        printf(SEPARATION);
        printf("Freeing the head\n---\n");
        free_head(new_head);
    }
    return 0;
}
*/