#include <stdio.h>
#include <stdlib.h>

#define FILENAME "grafo.txt"

void loadGraph(int ***mat, int *pn, int *pa);
int powerset(int pos, int *sol, int n, int cnt, int **mat, int nArchi);
int isVertexCover(int **mat, int nArchi, int *sol, int n);

int main(int argc, char const *argv[]) {
    int nNodi, nArchi, **mat, *sol;

    loadGraph(&mat, &nNodi, &nArchi);

    sol = (int *)calloc(nNodi, sizeof(int));
    printf("I vertex cover del grafo sono: \n");
    int cnt = powerset(0, sol, nNodi, 0, mat, nArchi);
    printf("In totale %d vertex cover\n", cnt);

    return 0;
}

// leggi grafo da file e alloca strutture dati
void loadGraph(int ***pmat, int *pn, int *pa) {
    FILE *f = fopen(FILENAME, "r");

    fscanf(f, "%d%d", pn, pa);
    *pmat = (int **)malloc(*pa * sizeof(int *));

    for (int i = 0; i < *pa; i++) {
        (*pmat)[i] = (int *)malloc(*pn * sizeof(int));
        fscanf(f, "%d%d", &(*pmat)[i][0], &(*pmat)[i][1]);
    }
    fclose(f);
}

int powerset(int pos, int *sol, int n, int cnt, int **mat, int nArchi) {
    int j;
    // condizione terminale
    if (pos >= n) {
        // controllo se il set trovato è un vertex cover
        if (isVertexCover(mat, nArchi, sol, n) == 1) {
            for (j = 0; j < n; j++)
                // siccome i nodi vanno da 0 a n, non occorre un vettore di partenza "val"
                // in quanto c'è corrispondenza tra l'indice di sol e il nodo rappresentato
                if (sol[j] != 0)
                    printf("%d", j);
            printf("\n");
            return cnt + 1;
        }
        return cnt;  // non aggiungo una soluzione
    }
    sol[pos] = 0;
    cnt = powerset(pos + 1, sol, n, cnt, mat, nArchi);
    sol[pos] = 1;
    cnt = powerset(pos + 1, sol, n, cnt, mat, nArchi);
    return cnt;
}

int isVertexCover(int **mat, int nArchi, int *sol, int n) {
    for (int i = 0; i < nArchi; i++) {
        // almeno uno dei due vertici deve appartenere al sottoinsieme
        // il nodo i-esimo appartiene all'insieme solo se l'i-esimo elemento di sol vale 1
        if (!(sol[mat[i][0]] || sol[mat[i][1]])) {
            return 0;
        }
    }
    return 1;  // soluzione valida
};