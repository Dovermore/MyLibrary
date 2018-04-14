typedef List Queue;

void init_queue(Queue *queue);

void free_queue(Queue* queue);

void queue_enqueue(Queue* queue, int data);

int queue_dequeue(Queue* queue);

int queue_size(Queue* queue);