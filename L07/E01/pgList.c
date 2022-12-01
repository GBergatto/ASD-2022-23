#include "pgList.h"

#include <stdlib.h>

typedef struct node *link;
struct node {
    pg item;
    link next;
};

struct pgList_s {
    link head;
    link tail;
    int N;
};

pgList list_init() {
    pgList list = (pgList)malloc(sizeof(*list));
    list->head = NULL;
    list->tail = NULL;
    list->N = 0;
    return list;
}

void pgList_free(pgList list) {
    link x = list->head, tmp;
    for (int i = 1; i < list->N; i++) {
        tmp = x;
        x = x->next;
        pg_clean(&(x->item));
        free(x);
    }
}

void pgList_read(FILE *fp, pgList list) {
    pg personaggio;
    while (pg_read(fp, &personaggio) == 1) {
        pgList_insert(list, personaggio);
    }
}

void pgList_print(FILE *fp, pgList list, invArray invArray) {
    if (list->head == NULL) return;
    for (link x = list->head; x != NULL; x = x->next) {
        pg_print(fp, &(x->item), invArray);
    }
}

link newNode(pg pers, link next) {
    link x = malloc(sizeof(*x));
    if (x == NULL)
        return NULL;
    else {
        x->item = pers;
        x->next = next;
    }
    return x;
}

void pgList_insert(pgList list, pg pers) {
    if (list->head == NULL) {  // testa e coda coincidono
        list->head = list->tail = newNode(pers, NULL);
    } else {
        list->tail->next = newNode(pers, NULL);
        list->tail = list->tail->next;
    }
}

pg *pgList_searchByCode(pgList list, char *key) {
    for (link x = list->head; x != NULL; x = x->next) {
        if (strcmp(x->item.cod, key) == 0)
            return &(x->item);
    }
    return NULL;
}

void pgList_remove(pgList list, char *key) {
    link x, p;
    for (x = list->head, p = NULL; x != NULL; p = x, x = x->next) {
        if (strcmp(x->item.cod, key) == 0) {
            if (list->head == x) {  // rimuovo il nodo puntato da head
                list->head = x->next;
            } else {
                p->next = x->next;
            }
            free(x);
            break;
        }
    }
}