#include "array.h"

#include <stdlib.h>

struct s_array {
    int size;
    int *items;
};

Array ARRAYinit(int size) {
    Array a;

    a = malloc(sizeof(*a));
    a->items = malloc(size * sizeof(int));
    a->size = size;
    return a;
}

void ARRAYfree(Array a) {
    free(a->items);
    free(a);
}

void ARRAYsetAtIndex(Array a, int id, int value) {
    if (id < 0 || id >= a->size) return;
    a->items[id] = value;
}

int ARRAYgetAtIndex(Array a, int id) {
    if (id < 0 || id >= a->size) return -1;
    return a->items[id];
}

int ARRAYsize(Array a) {
    return a->size;
}
