#include <stdio.h>
#include <stdlib.h>

#define FILENAME "elementi.txt"
#define DD 10
#define DP 20
#define N_DIAG 3
#define MAX_ELEMENTS 5

// genero il programma aggiungendo una diagonale alla volta
// modello disposizioni
// TROPPO LENTO

float bestScore = 0;

typedef struct {
    char nome[101];
    int tipo;
    int ingresso;
    int uscita;
    int precedenza;
    int finale;
    float valore;
    int difficolta;
} elemento;

typedef struct {
    int elementi[MAX_ELEMENTS];
    int n;
} diag;

enum tipo { transizione,
            indietro,
            avanti };

int loadData(elemento **pArr) {
    FILE *f = fopen(FILENAME, "r");
    int n;

    fscanf(f, "%d", &n);
    *pArr = (elemento *)malloc(n * sizeof(elemento));
    for (int i = 0; i < n; i++) {
        fscanf(f, "%s %d%d%d %d%d %f %d", (*pArr)[i].nome, &(*pArr)[i].tipo, &(*pArr)[i].ingresso,
               &(*pArr)[i].uscita, &(*pArr)[i].precedenza, &(*pArr)[i].finale, &(*pArr)[i].valore, &(*pArr)[i].difficolta);
    }
    fclose(f);
    return n;
}

void buildDiag(int dPos, int ePos, diag prog[], diag bestProg[], int n, elemento *elementi, int totDiff, int diagDiff) {
    int valid;

    if (ePos == MAX_ELEMENTS) {  //  Diagonale piena. Passo alla successiva
        // check sulla diagonale
        if (diagDiff <= DD)
            buildDiag(dPos + 1, 0, prog, bestProg, n, elementi, totDiff, 0);
        return;
    }
    if (dPos == N_DIAG) {  // terminazione: il programma è al completo
        if (totDiff > DP) return;

        int fAvanti = 0, fIndietro = 0, fLenGt2 = 0, fBonus = 0;
        float score = 0, scoreLastDiag = 0;
        elemento el;
        for (int k = 0; k < N_DIAG; k++) {
            if (prog[k].n >= 2) fLenGt2 = 1;  // almeno una diagonale con n>=2

            for (int j = 0; j < prog[k].n; j++) {
                el = elementi[prog[k].elementi[j]];
                score += el.valore;

                // almeno un elemento avanti e uno indietro
                if (el.tipo == avanti)
                    fAvanti++;
                else if (el.tipo == indietro)
                    fIndietro++;

                // bonus se includo un elemento difficile nell'ultima diag
                if (k == N_DIAG - 1) {
                    scoreLastDiag += el.valore;
                    if (el.difficolta >= 8) fBonus = 1;
                }
            }
        }

        if (!(fAvanti && fIndietro && fLenGt2)) return;

        score += scoreLastDiag * 0.5 * (float)fBonus;
        if (score > bestScore) {
            printf("%f\n", score);
            bestScore = score;
            for (int k = 0; k < N_DIAG; k++) {
                bestProg[k].n = prog[k].n;
                for (int j = 0; j < prog[k].n; j++) {
                    bestProg[k].elementi[j] = prog[k].elementi[j];
                }
            }
        }

        return;
    }

    for (int i = 0; i < n; i++) {
        valid = 1;
        if (ePos == 0) {
            valid = valid && (elementi[i].precedenza == 0);  // puo essere eseguito per primo
            valid = valid && (elementi[i].ingresso == 1);    // ingresso frontale
        } else {
            elemento prec = elementi[prog[dPos].elementi[ePos - 1]];
            valid = valid && (prec.finale == 0);                         // il precedente non è finale
            valid = valid && (prec.uscita == elementi[i].ingresso);      // compatibilità uscita-ingresso
            valid = valid && (diagDiff + elementi[i].difficolta <= DD);  // limite di punti per la diagonale
        }

        if (valid) {
            // aggiungo l'elemento alla diagonale corrente
            prog[dPos].elementi[ePos] = i;
            prog[dPos].n++;
            buildDiag(dPos, ePos + 1, prog, bestProg, n, elementi, totDiff + elementi[i].difficolta, diagDiff + elementi[i].difficolta);
            prog[dPos].elementi[ePos] = -1;
            prog[dPos].n--;
        }
    }
    // oppure passo alla diag successiva, se quella corrente non è vuota
    if (ePos > 0)
        buildDiag(dPos + 1, 0, prog, bestProg, n, elementi, totDiff, 0);
    return;
}

int main(int argc, char const *argv[]) {
    int n;
    elemento *arr;

    diag prog[N_DIAG], best[N_DIAG];
    for (int i = 0; i < N_DIAG; i++) {
        prog[i].n = 0;
        best[i].n = 0;
        //? turn all elements to -1
    }

    n = loadData(&arr);
    buildDiag(0, 0, prog, best, n, arr, 0, 0);

    printf("%f\n", bestScore);
    int diff = 0;
    for (int k = 0; k < N_DIAG; k++) {
        for (int j = 0; j < best[k].n; j++) {
            diff += arr[best[k].elementi[j]].difficolta;
            printf("%s %.3f ", arr[best[k].elementi[j]].nome, arr[best[k].elementi[j]].valore);
        }
        printf("\n");
    }
    printf("\n%d\n", diff);

    free(arr);

    return 0;
}
