#include "symbolTable.h"

struct symboltable {
    Vert *arr;
    int maxN;  // massima capienza
    int size;  // dimensione corrente
};

ST STinit(int maxN) {
    ST st;
    st = (ST)malloc(sizeof(*st));
    st->arr = malloc(maxN * sizeof(Vert));

    st->maxN = maxN;
    st->size = 0;
}

void STfree(ST st) {
    free(st->arr);
    free(st);
}

int STcount(ST st) {
    return st->size;
}

void STinsert(ST st, Vert val) {
    int i = st->size++;
    if (st->size > st->maxN) {
        st->arr = realloc(st->arr, (2 * st->maxN) * sizeof(Vert));
        if (st->arr == NULL)
            return;
        st->maxN = 2 * st->maxN;
    }
    int res = strcmp(val.id, st->arr[i - 1].id);
    while ((i > 0) && strcmp(val.id, st->arr[i - 1].id) < 0) {
        st->arr[i] = st->arr[i - 1];
        i--;
    }
    st->arr[i] = val;
}

void STprint(ST st) {
    for (int i = 0; i < st->size; i++) {
        printf("%s ", st->arr[i].id);
    }
    printf("\n\n");
}

Vert STsearchByIndex(ST st, int index) {
    return st->arr[index];
}

int STsearchByName(ST st, char k[]) {
    int l = 0, r = st->size - 1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (strcmp(st->arr[m].id, k) == 0)
            return m;

        if (strcmp(st->arr[m].id, k) > 0)
            r = m - 1;
        else
            l = m + 1;
    }
    return -1;
}