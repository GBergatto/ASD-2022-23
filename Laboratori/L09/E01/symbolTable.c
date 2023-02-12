#include "symbolTable.h"

#include <stdlib.h>
#include <string.h>

struct symboltable {
    char **arr;
    int maxN;  // massima capienza
    int size;  // dimensione corrente
};

static int search(ST st, char *name);

ST STinit(int maxN) {
    ST st = malloc(sizeof(struct symboltable));
    st->arr = malloc(maxN * sizeof(char *));
    st->size = 0;
    st->maxN = maxN;
    return st;
}

int STgetIndex(ST st, char *name) {
    int index = search(st, name);
    return index;
}

void STinsert(ST st, char *name) {
    st->arr[st->size++] = strdup(name);
}

char *STgetName(ST st, int index) {
    return st->arr[index];
}

void STfree(ST st) {
    for (int i = 0; i < st->size; i++)
        free(st->arr[i]);
    free(st->arr);
    free(st);
}

static int search(ST st, char *name) {
    for (int i = 0; i < st->size; i++) {
        if (strcmp(name, st->arr[i]) == 0)
            return i;
    }
    return -1;
}