/* Header guard */
#ifndef _POLY_SORTED_LIST_H
#define _POLY_SORTED_LIST_H



/* prototypes below */

/*
 * return a new head of the list
 */
head_t *make_new_head(int (*cmpfunc)(void*, void*),
                                            void (*func_print)(void*));

/*
 * Insert a particular key to the list
 */
head_t *insert_key(void *key, head_t *head);

/*
 * Print the whole list
 */
void print_whole_head(head_t *head);

/*
 * Free the list
 */
void free_head(head_t *head);

/*
 * Delete the first element
 */
head_t *delete_first(head_t *head);

/*
 * Delete the last element
 */
head_t *delete_last(head_t *head);
/* prototypes above 


#endif