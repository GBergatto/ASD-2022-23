#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define DBG 0  // stampa tutte le soluzioni possibili
#define FILENAME "griglia1.txt"

// 1. STRUTTURA DATI
// schema di gioco
// matrice di interi allocata dinamicamente

int **leggiMappa(FILE *fin, int *R, int *C);

int **leggiMappa(FILE *fin, int *R, int *C) {
    int **grid;
    fscanf(fin, "%d%d", R, C);

    grid = (int **)malloc((*R) * sizeof(int *));
    for (int i = 0; i < *R; i++) {
        grid[i] = malloc((*C) * sizeof(int));
        for (int j = 0; j < *C; j++) {
            fscanf(fin, "%d", &grid[i][j]);
        }
    }
    return grid;
}

// 2. PROBLEMA DI VERIFICA
// suppongo fn = file name

// visito ogni regione tramite DFS e conto quanti elementi la compongono
// se il numero di elementi è diverso dalla cifra che compone la regione,
// la griglia non è valida

void DFS(int **m, int **mark, int r, int c, int rows, int cols, int *cnt);
int checkSolution(int **m, int R, int C);

int verifica(char *fn, int **mappa) {
    FILE *f = fopen(fn, "r");
    int R, C, val, cnt;
    mappa = leggiMappa(f, &R, &C);

    return checkSolution(mappa, R, C);
}

// return 1 if valid
int checkSolution(int **m, int R, int C) {
    int cnt, val;
    int **mark = malloc(R * sizeof(int *));  // segna le celle visite
    for (int l = 0; l < R; l++)
        mark[l] = calloc(C, sizeof(int));

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            cnt = 1;  // dimensione della regione
            val = m[i][j];

            if (mark[i][j] == 0) {
                mark[i][j] = 1;
                DFS(m, mark, i, j, R, C, &cnt);

                if (val != cnt)
                    return 0;  // non valida
            }
        }
    }
    return 1;  // valida
}

// esplora e conta tutte le celle adiacenti con lo stesso valore
void DFS(int **m, int **mark, int r, int c, int rows, int cols, int *cnt) {
    int dr[4] = {0, 1, 0, -1};
    int dc[4] = {1, 0, -1, 0};

    for (int i = 0; i < 4; i++) {  // celle adiacenti nelle 4 direzioni
        if (r + dr[i] < rows && r + dr[i] >= 0 && c + dc[i] < cols && c + dc[i] >= 0) {
            if (mark[r + dr[i]][c + dc[i]] == 0) {  // non ancora visitata
                if (m[r + dr[i]][c + dc[i]] == m[r][c]) {
                    (*cnt)++;
                    mark[r + dr[i]][c + dc[i]] = 1;
                    DFS(m, mark, r + dr[i], c + dc[i], rows, cols, cnt);
                }
            }
        }
    }
}

// 3. PROBLEMA DI RICERCA
// Soluzione valida che minimizza il numero di cifre diverse utilizzate
// Esploro tutto lo spazio delle soluzioni

int minDD = INT_MAX;

int countDifferentDigits(int **m, int R, int C);
void disp(int pos, int **sol, int **bestSol, int R, int C);

void solve(int **m, int R, int C) {
    int **bestSol = malloc(R * sizeof(int *));
    for (int i = 0; i < R; i++)
        bestSol[i] = malloc(C * sizeof(int));

    disp(0, m, bestSol, R, C);
    printf("cifre distinte = %d\n", minDD);
    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++)
            printf("%d ", bestSol[i][j]);
        printf("\n");
    }
    printf("\n");
}

int countDifferentDigits(int **m, int R, int C) {
    int cnt = 0, flag, arr[10];

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            flag = 0;
            for (int k = 0; !flag && k < cnt; k++) {
                if (m[i][j] == arr[k]) flag = 1;
            }
            if (flag == 0)  // nuova cifra
                arr[cnt++] = m[i][j];
        }
    }
    return cnt;
}

// modello disposizioni con ripetizioni
void disp(int pos, int **sol, int **bestSol, int R, int C) {
    if (pos >= R * C) {
        if (checkSolution(sol, R, C)) {
            int dd = countDifferentDigits(sol, R, C);

#if DBG
            printf("%d\n", dd);
            for (int i = 0; i < R; i++) {
                for (int j = 0; j < C; j++)
                    printf("%d ", sol[i][j]);
                printf("\n");
            }
            printf("\n");
#endif

            if (dd < minDD) {  // aggiorno soluzione migliore
                minDD = dd;
                for (int i = 0; i < R; i++) {
                    for (int j = 0; j < C; j++) {
                        bestSol[i][j] = sol[i][j];
                    }
                }
            }
        }
        return;
    }

    if (sol[pos / C][pos % C] == 0) {
        for (int i = 1; i <= 9; i++) {
            sol[pos / C][pos % C] = i;
            disp(pos + 1, sol, bestSol, R, C);
            sol[pos / C][pos % C] = 0;
        }
    } else {  // valore prestabilito
        disp(pos + 1, sol, bestSol, R, C);
    }
    return;
}

// TEST
int main() {
    FILE *f = fopen("griglia.txt", "r");
    int R, C, **mat, **mat2;
    mat = leggiMappa(f, &R, &C);

    // 2. verifica
    int res = verifica(FILENAME, mat2);
    if (res) {
        printf("Valida\n\n");
    } else
        printf("Non valida\n\n", res);

    // 3. ricerca
    solve(mat, R, C);
}
