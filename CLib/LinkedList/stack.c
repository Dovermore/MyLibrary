/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: stack.c
 
 *This program is created for the purpose of:
    workshop 3 make file stack implementation
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include "list.h"
#include "stack.h"


void init_stack(Stack **stack) {
    init_list(stack);
}

void free_stack(Stack *stack) {
    free_list(stack);
}

void push(Stack *stack, int data) {
    list_add_end(stack, data);
}

int pop(Stack *stack) {
    return list_remove_end(stack);
}

int stack_size(Stack *stack) {
    return list_size(stack);
}

int empty_stack(Stack *stack) {
    return !stack_size(stack);
}