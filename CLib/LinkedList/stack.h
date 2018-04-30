/* Header guard */
#ifndef _STACK_H
#define _STACK_H


typedef List Stack;

void init_stack(Stack **stack);

void free_stack(Stack *stack);

void push(Stack *stack, int data);

int pop(Stack *stack);

int stack_size(Stack *stack);

int empty_stack(Stack *stack);


#endif