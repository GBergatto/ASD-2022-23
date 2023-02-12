#include "invArray.h"

#include <stdlib.h>

struct invArray_s {
    inv* a;
    int N;
};

invArray invArray_init() {
    invArray a = (invArray)malloc(sizeof(*a));
    a->a = NULL;
    a->N = 0;
    return a;
}

void invArray_free(invArray array) {
    free(array->a);
    free(array);
}

void invArray_read(FILE* fp, invArray array) {
    inv obj;
    fscanf(fp, "%d", &(array->N));
    array->a = (inv*)malloc(array->N * sizeof(inv));

    for (int i = 0; i < array->N; i++) {
        inv_read(fp, &obj);
        array->a[i] = obj;
    }
}

void invArray_print(FILE* fp, invArray array) {
    for (int i = 0; i < array->N; i++) {
        inv_print(fp, &(array->a[i]));
    }
}

void invArray_printByIndex(FILE* fp, invArray array, int index) {
    inv_print(fp, &(array->a[index]));
}

inv* invArray_getByIndex(invArray array, int index) {
    return &(array->a[index]);
}

int invArray_searchByName(invArray array, char* name) {
    for (int i = 0; i < array->N; i++) {
        if (strcmp(array->a[i].nome, name) == 0)
            return i;
    }
    return -1;
}