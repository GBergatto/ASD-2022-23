#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#define FILENAME "grafo0.txt"
#define DBG 0

void generateMinEdgesSets(int pos, int k, int start, int nEdges, GRAPH g, int *found, int *maxWeight, int *mark, int *bestSol, int *pre, int *post);

int main() {
    int *mark;     // vettore per segnare quali archi eliminare
    int *bestSol;  // insieme a peso massimo da eliminare
    int found = 0, maxWeight = 0, *pre, *post, nEdges, nVert;
    FILE *f;
    GRAPH g;

    f = fopen(FILENAME, "r");
    if (f == NULL) {
        printf("Errore nella lettura del file\n");
        return 1;
    }

    g = GRAPHload(f);
    fclose(f);

    nEdges = GRAPHcountEdges(g);
    nVert = GRAPHcountVert(g);

    mark = calloc(nEdges, sizeof(int));
    bestSol = malloc(nEdges * sizeof(int));
    pre = malloc(nVert * sizeof(int));
    post = malloc(nVert * sizeof(int));

    if (GRAPHisDag(g, mark, pre, post)) {
        printf("Il grafo era già un DAG. Non occorre rimuovere alcun arco.\n\n");
    } else {
        printf("Soluzioni possibili:\n");
        // costruisco sottoinsiemi di archi con cardinalità k crescente
        // appena trovo almeno un sottoinsieme valido, ho la cardinalità minima
        int k;
        for (k = 1; k < nEdges && !found; k++) {
            generateMinEdgesSets(0, k, 0, nEdges, g, &found, &maxWeight, mark, bestSol, pre, post);
        }
        k--;  // il for incrementa k anche in uscita
        printf("\nCardinalità minina = %d\n\n", k);
        printf("Insieme a peso massimo:\n");
        for (int i = 0; i < nEdges; i++) {
            if (bestSol[i]) {
                GRAPHprintEdge(g, i);
                printf("  ");
            }
        }
        printf("\tpeso = %d\n\n", maxWeight);

        // rimuovo gli archi dell'insieme a peso massimo per ottenere il DAG
        GRAPHremoveEdges(g, bestSol, 1);
        // GRAPHprintListAdj(g);
    }

    free(pre);
    free(post);

    // calcolo delle distanze massime da ogni nodo sorgente verso ogni nodo del DAG
    GRAPHdagMaximumPath(g);

    GRAPHfree(g);
    free(mark);
    free(bestSol);
}

// Genera tutti i sottoinsiemi di archi di cardinalità k e verifica se il grafo rimanente è un DAG
void generateMinEdgesSets(int pos, int k, int start, int nEdges, GRAPH g, int *found, int *maxWeight, int *mark, int *bestSol, int *pre, int *post) {
    if (pos == k) {
        if (GRAPHisDag(g, mark, pre, post)) {
            *found = 1;

            for (int i = 0; i < nEdges; i++) {
                if (mark[i]) {
                    GRAPHprintEdge(g, i);
                    printf("  ");
                }
            }

            int weight = GRAPHgetWeight(g, mark);
            printf("\tpeso = %d\n", weight);
            if (weight > *maxWeight) {
                *maxWeight = weight;
                for (int i = 0; i < nEdges; i++)
                    bestSol[i] = mark[i];
            }
        }
        return;
    }

    // modello delle combinazioni semplici
    for (int i = start; i < nEdges; i++) {
        mark[i] = 1;
        generateMinEdgesSets(pos + 1, k, i + 1, nEdges, g, found, maxWeight, mark, bestSol, pre, post);
        mark[i] = 0;
    }
}
