#include <stdio.h>
#include <stdlib.h>

#define DBG 0  // per print aggiuntive
#define DD 7
#define DP 30

#define N_DIAG 3
#define MAX_ELEMENTS 5
#define FILENAME "elementi.txt"

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
    int difficolta;
    float valore;
} diag;

enum tipo { transizione,
            indietro,
            avanti };

void buildDiagonals(int pos, diag **diagonali, int *maxD, diag *d, int n, elemento *elementi, int *cnt);
void buildProgramm(int pos, diag *val, diag *prog, diag *bestProg, elemento *elementi, int n, int start, int difficulty);

// leggi elementi da file
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
    diag prog[N_DIAG], best[N_DIAG];
    int cnt = 0, maxD = 3;
    diag *diagonali = (diag *)malloc(maxD * sizeof(diag));
    diag tmp;

    // azzero la diagonale temporanea
    tmp.n = 0;
    tmp.difficolta = 0;
    tmp.valore = 0;

    for (int i = 0; i < N_DIAG; i++) {
        prog[i].n = 0;
        best[i].n = 0;
    }

    n = loadData(&elementi);

    // genero tutte le diagonali valide
    buildDiagonals(0, &diagonali, &maxD, &tmp, n, elementi, &cnt);
    printf("Costruite %d diagonali valide\n", cnt);

    // compongo il progrogramma migliore
    buildProgramm(0, diagonali, prog, best, elementi, cnt, 0, 0);

    // stampa la soluzione migliore
    int diff = 0;
    elemento e;
    for (int k = 0; k < N_DIAG; k++) {
        printf("\ndiag #%d: ", k + 1);
        for (int j = 0; j < best[k].n; j++) {
            e = elementi[best[k].elementi[j]];
            diff += e.difficolta;
            printf("%24s %02.3f ", e.nome, e.valore);
        }
    }
    printf("\n\nPunteggio: %.3f\nDifficoltà del programma: %d\n", bestScore, diff);
    free(elementi);
    return 0;
}

// genero un elenco di tutte le diagonali valide
void buildDiagonals(int pos, diag **diagonali, int *maxD, diag *d, int n, elemento *elementi, int *cnt) {
    int valid;

    if (pos == MAX_ELEMENTS) return;  // diagonale piena

    if (pos > 0) {
        int flag = 0;  // prendo tutte le diagonali con almeno un elemento

        // almeno un elemento che non sia una transizione
        for (size_t k = 0; k < pos; k++) {
            if (elementi[d->elementi[k]].tipo != transizione) {
                flag = 1;
                break;
            }
        }

        if (flag) {
            if (*cnt == (*maxD - 1)) {
                // realloco
                *maxD *= 2;
                (*diagonali) = (diag *)realloc(*diagonali, (*maxD) * sizeof(diag));
            }

            d->n = pos;
            (*diagonali)[(*cnt)] = *d;
            (*cnt)++;

#if DBG
            for (int l = 0; l < pos; l++) {
                printf("%s ", elementi[d->elementi[l]].nome);
            }
            printf("\n");
#endif
        }
    }

    for (int i = 0; i < n; i++) {
        valid = 1;
        if (pos == 0) {
            valid = valid && (elementi[i].precedenza == 0);  // puo essere eseguito per primo
            valid = valid && (elementi[i].ingresso == 1);    // ingresso frontale
        } else {
            elemento prec = elementi[d->elementi[pos - 1]];
            valid = valid && (prec.finale == 0);                              // il precedente non è finale
            valid = valid && (prec.uscita == elementi[i].ingresso);           // compatibilità uscita-ingresso
            valid = valid && (d->difficolta + elementi[i].difficolta <= DD);  // limite di punti per la diagonale
        }

        if (valid) {
            // aggiungo l'elemento alla diagonale corrente
            d->elementi[pos] = i;
            d->valore += elementi[i].valore;
            d->difficolta += elementi[i].difficolta;
            buildDiagonals(pos + 1, diagonali, maxD, d, n, elementi, cnt);
            d->elementi[pos] = -1;
            d->valore += elementi[i].valore;
            d->difficolta -= elementi[i].difficolta;
        }
    }
    return;
}

// scelgo N_DIAG diagonali tra quelle valide per costruire il programma migliore
// modello delle disposizioni con ripetizioni (posso inserire la stessa diagonale più volte)
void buildProgramm(int pos, diag *val, diag *prog, diag *bestProg, elemento *elementi, int n, int start, int difficulty) {
    if (pos >= N_DIAG) {
        if (difficulty > DP) return;

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
#if DBG
            printf("%f\n", score);
#endif
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
        prog[pos] = val[i];
        buildProgramm(pos + 1, val, prog, bestProg, elementi, n, i + 1, difficulty + val[i].difficolta);
    }
    return;
}
