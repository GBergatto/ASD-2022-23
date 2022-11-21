#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "att2.txt"
#define DBG 0

typedef struct
{
    int inizio;
    int fine;
    int durata;
} att;

int loadData(att **pv);
int cmpfunc(const void *a, const void *b);
void printSol(att *val, int *P, int i);

int loadData(att **pv) {
    int n;
    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) return -1;

    fscanf(f, "%d", &n);
    *pv = (att *)malloc(n * sizeof(att));

    for (int i = 0; i < n; i++) {
        fscanf(f, "%d%d", &(*pv)[i].inizio, &(*pv)[i].fine);
        (*pv)[i].durata = (*pv)[i].fine - (*pv)[i].inizio;
    }
    return n;
}

int cmpfunc(const void *a, const void *b) {
    const int *A = a, *B = b;

    att val1 = *(att *)a;
    att val2 = *(att *)b;

    return (val1.fine - val2.fine);
}

void printSol(att *val, int *P, int i) {
    if (P[i] == -1) {
        printf("(%d-%d) ", val[i].inizio, val[i].fine);
        return;
    }
    printSol(val, P, P[i]);
    printf("(%d-%d) ", val[i].inizio, val[i].fine);  // in salita
    return;
}

void solveDP(att *val, int n) {
    int L[n], P[n], len = val[0].durata, last = 1;
    L[0] = val[0].durata, P[0] = -1;

    for (int i = 1; i < n; i++) {
        L[i] = val[i].durata;
        P[i] = -1;
        for (int j = 0; j < i; j++) {  // guardo indietro
            if ((val[j].fine <= val[i].inizio) && (L[i] <= val[i].durata + L[j])) {
                // aggiungo l'attività corrente alla sequenza di attività
                L[i] = val[i].durata + L[j];
                P[i] = j;
            }
        }
        if (L[i] > len) {  // aggiorno la miglior sequenza
            len = L[i];
            last = i;
        }
#if DBG
        printf("\nval: ");
        for (int k = 0; k < i; k++) printf("(%d-%d) ", val[k].inizio, val[k].fine);
        printf("\nL: ");
        for (int k = 0; k < i; k++) printf("%d ", L[k]);
        printf("\nP: ");
        for (int k = 0; k < i; k++) printf("%d ", P[k]);
        printf("\n");
#endif
    }

    printf("La durata totale massima vale %d e si ottiene con le seguenti attività\n", len);
    printSol(val, P, last);
    printf("\n");
}

int main(int argc, char const *argv[]) {
    att *v;
    int n;

    n = loadData(&v);
    // ordino le attività rispetto all'ora di fine
    qsort(v, n, sizeof(att), cmpfunc);

#if DBG
    for (int i = 0; i < n; i++) {
        printf("%d-%d (%d)\n", v[i].inizio, v[i].fine, v[i].durata);
    }
#endif

    solveDP(v, n);

    free(v);
    return 0;
}
