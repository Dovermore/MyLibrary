typedef List Queue;

void init_queue(Queue **queue);

void free_queue(Queue *queue);

void enqueue(Queue *queue, int data);

int dequeue(Queue *queue);

int queue_size(Queue *queue);

int empty_queue(Queue *queue);