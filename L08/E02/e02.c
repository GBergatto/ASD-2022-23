#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "symbolTable.h"

int main(int argc, char const *argv[]) {
    FILE *f = fopen(argv[1], "r");
    printf("%s\n", argv[1]);

    if (f == NULL) {
        printf("Errore nella lettura del file\n");
        return 1;
    }

    Graph g = GRAPHload(f);

    printf("Opzioni:\n1. elencare vertici\n2. verificare 3 vertici adiacenti\n3. generare lista delle adiacenze\n0. Quit\n");

    int opt;
    do {
        printf("$ ");
        scanf("%d", &opt);

        switch (opt) {
            case 1:
                // lista dei vertici in ordine alfabetico
                printf("Lista dei vertici in ordine alfabetico\n");
                GRAPHlistNodes(g, stdout);

                break;
            case 2:
                // controlla sottografo completo per 3 vertici da tastiera
                int res;
                char n1[MAXLEN], n2[MAXLEN], n3[MAXLEN];
                printf("Inserisci 3 vertici: ");
                scanf("%s%s%s", n1, n2, n3);

                res = GRAPHcheckAdjacencyMat(g, n1, n2, n3);
                printf("Usando la matrice delle adiacenze...\n");
                if (res == 1)
                    printf("I nodi %s, %s, %s formano un sottografo completo\n", n1, n2, n3);
                else if (res == 0)
                    printf("I nodi %s, %s, %s NON formano un sottografo completo\n", n1, n2, n3);
                else
                    printf("Errore! Almeno uno dei tre nodi inseriti non esiste\n");

                res = GRAPHcheckAdjacencyList(g, n1, n2, n3);
                if (res == -2) {
                    printf("\nLista delle adiacenze non ancora generata.\n");
                } else {
                    printf("\nUsando la lista delle adiacenze...\n");
                    if (res == 1)
                        printf("I nodi %s, %s, %s formano un sottografo completo\n", n1, n2, n3);
                    else if (res == 0)
                        printf("I nodi %s, %s, %s NON formano un sottografo completo\n", n1, n2, n3);
                    else
                        printf("Errore! Almeno uno dei tre nodi inseriti non esiste\n");
                }
                break;
            case 3:
                // genera la lista adiacenze
                GRAPHgenerateListAdj(g);
                break;
            case 0:
                printf("Quitting...\n");
                break;

            default:
                printf("Scelta non valida '%d'\n", opt);
                break;
        }

    } while (opt != 0);

    return 0;
}
