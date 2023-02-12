#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBG 0
#define DD 10
#define DP 20

#define MAX_ELEMENTS 5
#define N_DIAG 3
#define FILENAME "elementi.txt"

enum tipo { transizione,
            indietro,
            avanti };

typedef struct s_parametri {
    int serveAcrobInDiag;  // almeno un elemento acrobatico per diag
    int serveAvanti;       // almeno un elemento avanti nel programma
    int serveIndietro;     // almeno un elemento indietro nel programma
    int serveAcrobInSeq;   // almeno 2 elementi acrobatici consecutivi nel programma
} parametri;

typedef struct s_elemento {
    char nome[31];
    int tipo;
    int ingresso;
    int uscita;
    int precedenza;
    int finale;
    float valore;
    int difficolta;
} elemento;

int loadData(elemento **el);
void buildProgrammGreedy(elemento *elementi, int N);
int findValidElements(elemento *validArr, elemento *elementi, int N, elemento *sol, int pos, int col, int row, int diffDiags[], int diffTot);
void customSort(elemento arr[], int n, parametri param, int row, int col, int diffDiag, int followsAcr);
float objFunction(elemento el, parametri param, int row, int col, int diffDiag, int followsAcr);
void printProgramm(elemento *sol, int N);

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

int main(int argc, char const *argv[]) {
    int n;
    elemento *elementi;

    n = loadData(&elementi);
    buildProgrammGreedy(elementi, n);

    free(elementi);
    return 0;
}

void buildProgrammGreedy(elemento *elementi, int N) {
    int diffDiags[N_DIAG];
    int i, pos, diffTot = 0, nValid;
    elemento *validElements, *sol;
    parametri params[N_DIAG];
    int diagTerminata[N_DIAG];
    int found;

    // allocazione e valori di default
    sol = (elemento *)malloc(N_DIAG * MAX_ELEMENTS * sizeof(elemento));
    for (i = 0; i < N_DIAG * MAX_ELEMENTS; i++)
        strcpy(sol[i].nome, "");
    validElements = (elemento *)malloc(N * sizeof(elemento));

    for (i = 0; i < N_DIAG; i++) {
        diffDiags[i] = 0;
        diagTerminata[i] = 0;
        params[i].serveAcrobInSeq = params[i].serveAvanti = params[i].serveIndietro = params[i].serveAcrobInDiag = 1;
    }

    for (int col = 0; col < MAX_ELEMENTS; col++) {
        for (int row = 0; row < N_DIAG; row++) {
            pos = col + row * MAX_ELEMENTS;

            if (!diagTerminata[row]) {  // la diagonale corrente non termina con un elemento finale
                nValid = findValidElements(validElements, elementi, N, sol, pos, col, row, diffDiags, diffTot);

                if (nValid > 0) {
                    // ordino gli elementi validi per appetibilità decrescente => il primo è il migliore localmente
                    int segueAcr = (col > 0 && strcmp(sol[pos - 1].nome, "") && sol[pos - 1].tipo > 0);
                    customSort(validElements, nValid, params[row], row, col, diffTot, segueAcr);

#if DBG
                    for (int i = 0; i < nValid; i++) {
                        printf("%s %.2f %.3f\n", validElements[i].nome, validElements[i].valore, objFunction(validElements[i], params[row], row, col, diffTot, segueAcr));
                    }
                    printf("\n");
#endif
                    // scelta localmente ottima
                    sol[pos] = validElements[0];

                    // aggiorno difficoltà
                    diffDiags[row] += sol[pos].difficolta;
                    diffTot += sol[pos].difficolta;

                    // aggiorno i parametri della funzione obiettivo
                    if (sol[pos].tipo == indietro) {
                        params[row].serveAcrobInDiag = 0;
                        for (i = 0; i < N_DIAG; i++)
                            params[i].serveIndietro = 0;
                    }

                    if (sol[pos].tipo == avanti) {
                        params[row].serveAcrobInDiag = 0;
                        for (i = 0; i < N_DIAG; i++)
                            params[i].serveAvanti = 0;
                    }

                    if (sol[pos].tipo > 0 && sol[pos - 1].tipo > 0) {
                        // la diagonale corrente ha due elementi acrobatici consecutivi
                        params[row].serveAcrobInDiag = 0;
                        for (i = 0; i < N_DIAG; i++)
                            params[i].serveAcrobInSeq = 0;
                    }

                    if (sol[pos].finale) {
                        // elemento finale => conclude la diagonale corrente
                        diagTerminata[row] = 1;
                    }
#if DBG
                    printProgramm(sol, N_DIAG * MAX_ELEMENTS);
#endif
                }
            }
        }
    }

    printProgramm(sol, N_DIAG * MAX_ELEMENTS);

    free(validElements);
    free(sol);
    return;
}

// Genera lista degli elementi validi data la posizione corrente nel programma
int findValidElements(elemento *validArr, elemento *elementi, int n, elemento *sol, int pos, int col, int row, int diffDiags[], int diffTot) {
    int i, cnt = 0, valid;
    elemento el;

    for (i = 0; i < n; i++) {
        valid = 1;
        el = elementi[i];

        if (diffDiags[row] + elementi[i].difficolta > DD || diffTot + elementi[i].difficolta > DP)
            valid = 0;

        if (col == 0 && elementi[i].ingresso != 1)
            valid = 0;

        // direzioni di ingresso e uscita non combaciano
        if (col > 0 && elementi[i].ingresso != sol[pos - 1].uscita)
            valid = 0;

        if (col == 0 && elementi[i].precedenza)
            valid = 0;

        // se è rimasto valido dopo tutti i controlli, lo aggiungo alla lista
        if (valid) validArr[cnt++] = elementi[i];
    }

    return cnt;
}

// Ordina per appetibilità decrescente
void customSort(elemento arr[], int n, parametri param, int row, int col, int diffDiag, int followsAcr) {
    elemento tmp;
    float valA, valB;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            valA = objFunction(arr[j], param, row, col, diffDiag, followsAcr);
            valB = objFunction(arr[j + 1], param, row, col, diffDiag, followsAcr);

            if (valA < valB) {
                tmp = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

// Funzione obiettivo. Calcola l'appetibilità di un elemento in base al contesto
float objFunction(elemento el, parametri param, int row, int col, int diffTot, int followsAcr) {
    float value = el.valore * (col + 1) / el.difficolta;

    //* dipendenza dalla colonna corrente
    // più avanti sono, più è importante che tutti i requisiti siano soddisfatti

    // privilegia bonus sull'ultima riga
    if (col > 0 && el.difficolta >= 8) {
        if (row == N_DIAG - 1) {
            value += col * el.valore;
        } else {
            value -= col * el.valore;
        }
    }

    // inserire due elementi acrobatici di fila
    if (param.serveAcrobInSeq && followsAcr) {
        if (el.tipo > 0) {
            value += 3 * (col + 1);
        } else {
            value -= 3 * (col + 1);
        }
    }

    // inserire un elemento avanti e uno indietro
    if (param.serveAvanti && param.serveIndietro) {
        if (el.tipo > 0)
            value += 2 * (col + 1);
    } else {
        if ((param.serveAvanti && el.tipo == avanti) || (param.serveIndietro && el.tipo == indietro)) {
            // risolve il requisito di avanti/indietro
            value += 3 * (col + 1);
        } else {
            if (el.ingresso != el.uscita) {
                // non risolve il requisito, ma cambia direzione (avrò più possibilità al giro dopo)
                value += 0.5 * (col + 1);
            }
        }
    }

    // inserire un elemento acrobatico per ogni diagonale
    if (param.serveAcrobInDiag && el.tipo > 0) {
        value += col * col;
    }

    return value;
}

void printProgramm(elemento *sol, int N) {
    int pos, diff = 0;
    float parz = 0, tot = 0;
    for (int i = 0; i < N_DIAG; i++) {
        parz = 0;
        printf("diag #%d ", i + 1);
        for (int j = 0; j < MAX_ELEMENTS; j++) {
            pos = j + MAX_ELEMENTS * i;
            if (strcmp(sol[pos].nome, "")) {
                parz += sol[pos].valore;
                diff += sol[pos].difficolta;
                // se la diagonale si conclude con un elemento difficile => bonus
                if (i == N_DIAG - 1 && sol[pos].difficolta >= 8 && (strcmp(sol[pos + 1].nome, "") == 0 || j == MAX_ELEMENTS - 1)) {
                    parz *= 1.5;
                }
                printf("%26s %02.3f %2d ", sol[pos].nome, sol[pos].valore, sol[pos].difficolta);
            }
        }
        printf("\n");
        tot += parz;
    }

    printf("\n\nPunteggio: %.3f\nDifficoltà del programma: %d\n", tot, diff);
}