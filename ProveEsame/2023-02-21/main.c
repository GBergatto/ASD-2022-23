#include <stdio.h>
#include <stdlib.h>

#include "array.h"

#define FILE_NAME "input.txt"
#define DBG 0

int **malloc2d(int S);
void loadData(FILE *f, Array *p_persone, Array *p_incarichi, int ***pmat);
int min(int a, int b);
int solve(Array persone, Array incarichi, int **sinergia);
void solveR(int pos, int *sol, int *bestSol, int *bestScore, Array persone, Array incarichi, int **sinergia);

int main() {
    Array persone, incarichi;
    int **sinergia;

    FILE *f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        printf("Errore in lettura file\n");
    }

    // puntatore a puntatore per inizializzarli nella funzione dopo aver letto le dimensioni da file
    loadData(f, &persone, &incarichi, &sinergia);
    fclose(f);

    int score = solve(persone, incarichi, sinergia);
}

int **malloc2d(int S) {
    int **m;

    m = malloc(S * sizeof(int *));
    for (int i = 0; i < S; i++)
        m[i] = malloc(S * sizeof(int));

    return m;
}

void loadData(FILE *f, Array *p_persone, Array *p_incarichi, int ***p_mat) {
    int P, T, tmp, **mat;
    Array persone, incarichi;

    // leggo il vettore degli incarichi
    fscanf(f, "%d", &T);
    incarichi = ARRAYinit(T);
    for (int i = 0; i < T; i++) {
        fscanf(f, "%d", &tmp);
        ARRAYsetAtIndex(incarichi, i, tmp);
    }

    // leggo il vettore delle persone
    fscanf(f, "%d", &P);
    persone = ARRAYinit(P);
    for (int i = 0; i < P; i++) {
        fscanf(f, "%d", &tmp);
        ARRAYsetAtIndex(persone, i, tmp);
    }

    // leggo la matrice delle sinergie
    mat = malloc2d(P);
    for (int i = 0; i < P; i++)
        for (int j = 0; j < P; j++)
            fscanf(f, "%d", &mat[i][j]);

    // aggiorno le variabili della funzione chiamante
    *p_persone = persone;
    *p_incarichi = incarichi;
    *p_mat = mat;
}

int min(int a, int b) {
    if (a < b) return a;
    return b;
}

int checkSol(Array persone, Array incarichi, int *sol, int **sinergia) {
    int score = 0, I, P;

    I = ARRAYsize(incarichi);
    P = ARRAYsize(persone);

    // verifico che ogni persona sia assegnata ad un incarico
    for (int i = 0; i < P; i++) {
        if (sol[i] < 0 || sol[i] >= I)
            return -1;
    }

    // vettore che contiene le somme della forza lavoro per ogni incarico
    int *totali = calloc(I, sizeof(int));

    for (int j = 0; j < P; j++) {
        // aggiugo il contributo della j-esima persona all'incarico sol[j]
        totali[sol[j]] += ARRAYgetAtIndex(persone, j);

        for (int k = j + 1; k < P; k++) {
            if (sol[k] == sol[j])                  // se lavora allo stesso progetto
                totali[sol[j]] += sinergia[j][k];  // totali[sol[j]] anziché "tot"
        }
    }

    for (int i = 0; i < I; i++) {
        // se ho raggiunto la soglia
        if (totali[i] >= 0.75 * (float)ARRAYgetAtIndex(incarichi, i)) {
            score += min(totali[sol[i]], ARRAYgetAtIndex(incarichi, i));  // totali[sol[i]] anziché "tot"
        }
    }

    free(totali);
    return score;
}

int solve(Array persone, Array incarichi, int **sinergia) {
    int *sol = malloc(ARRAYsize(persone) * sizeof(int));
    int *bestSol = malloc(ARRAYsize(persone) * sizeof(int));
    int bestScore = 0;

    solveR(0, sol, bestSol, &bestScore, persone, incarichi, sinergia);

    printf("\nMiglior resa: %d\n", bestScore);
    for (int i = 0; i < ARRAYsize(persone); i++)
        printf("%d ", bestSol[i]);
    printf("\n");

    return bestScore;
}

void solveR(int pos, int *sol, int *bestSol, int *bestScore, Array persone, Array incarichi, int **sinergia) {
    if (pos >= ARRAYsize(persone)) {
        int score = checkSol(persone, incarichi, sol, sinergia);
        if (score > *bestScore) {
#if DBG
            printf("(%d) ", score);
#endif
            *bestScore = score;
            // copia sol in bestSol
            for (int i = 0; i < ARRAYsize(persone); i++) {
#if DBG
                printf("%d ", sol[i]);
#endif
                bestSol[i] = sol[i];  // avevo scambiato i due termini
            }
#if DBG
            printf("\n");
#endif
        }
        return;
    }

    for (int i = 0; i < ARRAYsize(incarichi); i++) {
        sol[pos] = i;
        solveR(pos + 1, sol, bestSol, bestScore, persone, incarichi, sinergia);
    }
}
