/*
 * Return an empty polymorphic tree with given compare function
 */
tree_t *return_empty_tree(int (*cmpfunc)(void*, void*));

/*
 * Find if certain key is in tree
 */
void *search_data(void *key, tree_t *tree);

/*
 * Insert a data to tree
 */
tree_t *insert_data(void *key, tree_t *tree);

/*
 * Recursive print the data
 */
void retrieve_print_data(tree_t *tree, void (*printfunc)(void*));

/*
 * Free the tree
 */
void free_tree(tree_t *tree);

/*
 * Insert array of sorted data to tree, which gives a tree with least height
 */
tree_t *insert_sorted_array_data(tree_t *tree, void**ary_data, size_t num_ele);

/*
 * Check if it's a binary tree
 */
int is_bst(tree_t *tree);

/*
 * Give the average depth of the tree
 */
double avg_depth(tree_t *tree);

/*
 * Give the depth of deepest node
 */
int depth_deepest_node(tree_t *tree);

/*
 * Delete a data in the tree
 */
tree_t *delete_data(tree_t *tree, void *key);

/*
 * Return the number of data in the tree
 */
int number_node(tree_t *tree);

/*
 * Merge Tree
 * Not yet implemented
 */
tree_t* merge_tree(tree_t* t1, tree_t* t2);