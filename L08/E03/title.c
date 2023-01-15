#include "title.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 21

// Title
// ADT di 1 classe per titoli

struct title {
    char name[MAXLEN];
    BST quotations;
};

Title TITLEsetVoid() {
    Title t = malloc(sizeof *t);
    strcpy(t->name, "");
    t->quotations = BSTinit();

    return t;
}

Title TITLEsetName(Title t, char name[]) {
    strcpy(t->name, name);
    return t;
}

char* TITLEgetName(Title t) {
    return t->name;
}

BST TITLEgetQuotations(Title t) {
    return t->quotations;
}

// Title Collections
// ADT di prima classe per collezione di titoli
// Linked list ordinata su name

typedef struct node* llink;  // puntatore a nodo
struct node {
    Title val;
    llink next;
};

struct tcollection {
    llink head;
    int N;
};

TC TCinit() {
    TC tc = malloc(sizeof(TC));
    tc->head = NULL;
    tc->N = 0;

    return tc;
}

int TCgetCount(TC tc) {
    return tc->N;
}

int TCprint(TC tc) {
    llink x;
    for (x = tc->head; x != NULL; x = x->next) {
        printf("%s\n", TITLEgetName(x->val));
    }
}

static llink newNode(Title val, llink next) {
    llink x = malloc(sizeof *x);
    if (x == NULL)
        return NULL;
    else {
        x->val = val;
        x->next = next;
    }
    return x;
}

Title TCsearch(TC tc, char k[]) {
    llink h = tc->head;
    for (llink x = h; x != NULL && strcmp(k, TITLEgetName(x->val)) >= 0; x = x->next)
        if (strcmp(TITLEgetName(x->val), k) == 0)
            return x->val;
    return TITLEsetVoid();
}

llink insertSorted(llink h, Title val) {
    llink x, p;
    char* k = TITLEgetName(val);
    if (h == NULL || strcmp(TITLEgetName(h->val), k) > 0)
        return newNode(val, h);
    for (x = h->next, p = h; x != NULL && strcmp(k, TITLEgetName(x->val)) > 0; p = x, x = x->next)
        ;
    p->next = newNode(val, x);

    return h;
}

void TCaddTitle(TC tc, Title val) {  // wrapper
    tc->head = insertSorted(tc->head, val);
    tc->N++;
}
