void heap_sort (void *base, size_t ntimes, size_t size,
                        int (*cmp)(void*, void*), void (*swap)(void*, void*));
void build_heap(void *base, size_t ntimes, size_t size,
                        int (*cmp)(void*, void*), void (*swap)(void*, void*));