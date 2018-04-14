/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: queue.c
 
 *This program is created for the purpose of:
    workshop 3 queue implementation using list
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include "list.h"
#include "queue.h"

void init_queue(Queue *queue) {
    return new_list(queue);
}

void free_queue(Queue* queue) {
    free_list(queue);
}

void queue_enqueue(Queue* queue, int data) {
    list_add_start(queue, data);
}

int queue_dequeue(Queue* queue) {
    return list_remove_end(queue);
}

int queue_size(Queue* queue) {
    return list_size(queue);
}