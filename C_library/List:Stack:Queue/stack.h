typedef List Stack;


void init_stack(Stack* stack);

void free_stack(Stack* stack);

void stack_push(Stack* stack, int data);

int stack_pop(Stack* stack);

int stack_size(Stack* stack);