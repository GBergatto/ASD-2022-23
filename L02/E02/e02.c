#include <stdio.h>
#include <stdlib.h>

#define FILENAME "mat.txt"

int **malloc2dR(int nr, int nc);
void malloc2dP(int ***mp, int nr, int nc);
void printMatr(int **m, int nr, int nc);
void stampaVettori(int *bianco, int *nero, int nr, int nc);
void separa(int **mat, int nr, int nc, int **bianco, int **nero);

int main(int argc, char const *argv[]) {
    int nr, nc;
    int **matr;          // matrice
    int *bianco, *nero;  // puntatori a vettore
    FILE *f = fopen(FILENAME, "r");

    fscanf(f, "%d%d", &nr, &nc);

    // matr = malloc2dR(nr, nc); // metodo alternativo
    malloc2dP(&matr, nr, nc);

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            fscanf(f, "%d", &matr[i][j]);
        }
    }

    printMatr(matr, nr, nc);
    separa(matr, nr, nc, &bianco, &nero);
    // passo un puntatore ai vettori bianco e nero
    // per poter modificare dalla funzione il valore dei due vettori

    stampaVettori(bianco, nero, nr, nc);
    free(bianco);
    free(nero);
    fclose(f);

    return 0;
}

void printMatr(int **m, int nr, int nc) {
    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

void stampaVettori(int *bianco, int *nero, int nr, int nc) {
    printf("\nBianco: ");
    for (int i = 0; i < (nc * nr + 1) / 2; i++) {
        printf("%d", bianco[i]);
    }
    printf("\nNero: ");
    for (int i = 0; i < (nc * nr) / 2; i++) {
        printf("%d", nero[i]);
    }
    printf("\n");
}

int **malloc2dR(int nr, int nc) {
    int **m = NULL;  // matrice = puntatore a vettore
    // vettore di puntatori (uno per riga)
    m = malloc(nr * sizeof(int *));
    // per ogni riga alloco un vettore di nc caselle
    for (int i = 0; i < nr; i++) {
        m[i] = malloc(nc * sizeof(int));
    }
    return m;
}

void malloc2dP(int ***mp, int nr, int nc) {
    *mp = malloc(nr * sizeof(int *));  // vettore di puntatori
    for (int i = 0; i < nr; i++) {
        (*mp)[i] = malloc(nc * sizeof(int));
    }
}

void separa(int **mat, int nr, int nc, int *pWhite[], int *pBlack[]) {
    int b = 0, n = 0;
    // len bianco = metà elementi (se dispari, arrotonda per eccesso)
    int *bianco = (int *)malloc(((nc * nr + 1) / 2) * sizeof(int));
    int *nero = (int *)malloc(((nc * nr) / 2) * sizeof(int));

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            if ((i + j) % 2 == 0) {
                bianco[b] = mat[i][j];
                b++;
            } else {
                nero[n] = mat[i][j];
                n++;
            }
        }
    }

    // aggiorno i due vettori del chiamamante tramite puntatore
    *pWhite = bianco;
    *pBlack = nero;
}