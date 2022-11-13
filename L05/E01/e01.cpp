#include <stdio.h>
#include <stdlib.h>

#define FILENAME "att1.txt"

typedef struct
{
    int inizio;
    int fine;
} att;

int maxDuration = 0;  // durata massima
int sizeFin;          // numero di attivita che formano la soluzione migliore

void attSel(int N, att *v);
int loadData(att **pv);
void disp(int pos, att *val, att *sol, int *mark, att *fin, int n, int k);

int loadData(att **pv) {
    int n;
    FILE *f = fopen(FILENAME, "r");
    if (f == NULL) {
        return -1;
    }

    fscanf(f, "%d", &n);
    *pv = (att *)malloc(n * sizeof(att));  // vettore di struct att
    for (int i = 0; i < n; i++) {
        fscanf(f, "%d%d", &(*pv)[i].inizio, &(*pv)[i].fine);
    }
    return n;
}

void attSel(int N, att *v) {
    att *sol = (att *)malloc(N * sizeof(att));
    att *fin = (att *)malloc(N * sizeof(att));
    int *mark = (int *)malloc(N * sizeof(int));

    // cerca disposizioni formate da un numero crescente di attività (k)
    for (int k = 0; k < N; k++) {
        disp(0, v, sol, mark, fin, N, k);
    }

    // stampa soluzione migliore
    printf("La durata totale massima vale %d e si ottiene con le seguenti attività:\n", maxDuration);
    for (int i = 0; i < sizeFin; i++)
        printf("(%d, %d)", fin[i].inizio, fin[i].fine);
    printf("\n");

    // dealloca
    free(sol);
    free(fin);
    free(v);
    free(mark);
}

void disp(int pos, att *val, att *sol, int *mark, att *fin, int n, int k) {
    if (pos >= k) {  // terminazione
        int d = 0;
        for (int i = 0; i < k; i++) {
            d += (sol[i].fine - sol[i].inizio);
        }
        if (d > maxDuration) {  // nuova soluzione ottimale
            maxDuration = d;
            sizeFin = k;
            for (int i = 0; i < k; i++) fin[i] = sol[i];
        }
        return;
    }
    for (int i = 0; i < n; i++) {
        int check = (mark[i] == 0);
        if (pos > 0) {
            // attivita con inizio successivo alla fine di quella precendente
            check = check && (val[i].inizio >= sol[pos - 1].fine);
        }

        if (check) {
            mark[i] = 1;
            sol[pos] = val[i];
            disp(pos + 1, val, sol, mark, fin, n, k);
            mark[i] = 0;
        }
    }
    return;
}

int main(int argc, char const *argv[]) {
    int n;
    att *v;

    n = loadData(&v);
    if (n == -1) {
        printf("Errore nella lettura del file\n");
        return 1;
    }

    attSel(n, v);
    return 0;
}
