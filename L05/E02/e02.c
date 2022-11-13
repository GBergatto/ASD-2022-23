#include <stdio.h>
#include <stdlib.h>

#define BOARD_FILE "board.txt"
#define TILES_FILE "tiles.txt"

typedef struct {
    char col1;
    int val1;  // in orizzontale
    char col2;
    int val2;  // in verticale
} tessera;

typedef struct {
    int index;  // indice della tessera, -1 se è libera
    int rot;    // 0 non ruotata, 1 ruotata
} cella;

int loadTiles(tessera **pv);
void loadBoard(cella ***pBoard, cella ***pBest, int *pr, int *pc, int **mark);
int punteggio(cella **board, tessera *tessere, int rows, int cols, int n);
void disp(int pos, cella **board, cella **best, tessera *tessere, int *mark, int rows, int cols, int n);
void printBoard(cella **board, tessera *tessere, int rows, int cols, int n);

int maxPoints = 0;

int loadTiles(tessera **pv) {
    int n;
    tessera *tessere;
    FILE *f = fopen(TILES_FILE, "r");
    fscanf(f, "%d ", &n);

    tessere = (tessera *)malloc(n * sizeof(tessera));
    for (int i = 0; i < n; i++) {
        fscanf(f, " %c %d %c %d", &tessere[i].col1, &tessere[i].val1, &tessere[i].col2, &tessere[i].val2);
    }

    *pv = tessere;
    fclose(f);
    return n;
}

void loadBoard(cella ***pBoard, cella ***pBest, int *pr, int *pc, int **mark) {
    int r, c;
    cella **board, **best;
    FILE *f = fopen(BOARD_FILE, "r");
    fscanf(f, "%d %d", &r, &c);

    board = (cella **)malloc(r * sizeof(cella *));
    best = (cella **)malloc(r * sizeof(cella *));
    for (int i = 0; i < r; i++) {
        board[i] = (cella *)malloc(c * sizeof(cella));
        best[i] = (cella *)malloc(c * sizeof(cella));
        for (int j = 0; j < c; j++) {
            fscanf(f, "%d/%d", &board[i][j].index, &board[i][j].rot);
            if (board[i][j].index != -1)
                (*mark)[board[i][j].index] = 1;  // tessera posizionata
        }
    }

    *pBoard = board;
    *pBest = best;
    *pr = r;
    *pc = c;
    fclose(f);
}

int punteggio(cella **board, tessera *tessere, int rows, int cols, int n) {
    int tot = 0, partial, valid, val;
    char colore, col;

    // percorro la scacchiera per righe
    for (int i = 0; i < rows; i++) {
        partial = 0;
        valid = 1;
        for (int j = 0; j < cols; j++) {
            if (board[i][j].rot == 1) {  // tessera ruotata
                col = tessere[board[i][j].index].col2;
                val = tessere[board[i][j].index].val2;
            } else {  // tessera non ruotata
                col = tessere[board[i][j].index].col1;
                val = tessere[board[i][j].index].val1;
            }

            if (j == 0) {
                // prima tessera => tutte le altre dovranno avere lo stesso colore
                colore = col;
                partial += val;
            } else {
                if (col == colore) {
                    partial += val;
                } else {  // colore diverso => la riga non conta
                    valid = 0;
                    break;
                }
            }
        }
        if (valid == 1) {
            tot += partial;
        }
    }

    // percorro la scacchiera per colonne
    for (int j = 0; j < cols; j++) {
        valid = 1;
        partial = 0;
        for (int i = 0; i < rows; i++) {
            if (board[i][j].rot == 1) {  // tessera ruotata
                col = tessere[board[i][j].index].col1;
                val = tessere[board[i][j].index].val1;
            } else {  // tessera non ruotata
                col = tessere[board[i][j].index].col2;
                val = tessere[board[i][j].index].val2;
            }

            if (i == 0) {
                // prima tessera => tutte le altre dovranno avere lo stesso colore
                colore = col;
                partial += val;
            } else {
                if (col == colore) {
                    partial += val;
                } else {  // colore diverso => la colonna non conta
                    valid = 0;
                    break;
                }
            }
        }

        if (valid == 1) {
            tot += partial;
        }
    }
    return tot;
}

void disp(int pos, cella **board, cella **best, tessera *tessere, int *mark, int rows, int cols, int n) {
    int r, c, score;
    if (pos >= rows * cols) {  // terminazione: abbastanza tessere per riempire la tavola
        score = punteggio(board, tessere, rows, cols, n);
        if (score > maxPoints) {  // aggiorna massimo e salva soluzione
            maxPoints = score;
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    best[i][j] = board[i][j];
        }
        return;
    }

    r = pos / rows;
    c = pos % rows;
    // se la casella [r][c] è gia occupata passa avanti senza fare nulla
    if (board[r][c].index != -1) {
        disp(pos + 1, board, best, tessere, mark, rows, cols, n);
        return;
    }
    for (int i = 0; i < n; i++) {
        // if (i == 8) {
        //     printBoard(board, tessere, rows, cols, n);
        //     printf("\n");
        // }

        if (mark[i] == 0) {
            mark[i] = 1;
            // aggiungi la cella NON RUOTATA al posto [r][c]
            board[r][c].index = i;
            board[r][c].rot = 0;
            disp(pos + 1, board, best, tessere, mark, rows, cols, n);
            // aggiungi la cella RUOTATA al posto [r][c]
            board[r][c].rot = 1;
            disp(pos + 1, board, best, tessere, mark, rows, cols, n);
            // devo liberare la casella!!!!!!
            board[r][c].index = -1;
            mark[i] = 0;
        }
    }
    return;
}

void printBoard(cella **board, tessera *tessere, int rows, int cols, int n) {
    int id;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            id = board[i][j].index;
            if (id == -1)
                printf("[   |   ] ");
            else {
                if (board[i][j].rot == 0)
                    printf("[%c%2d|%c%2d] ", tessere[id].col1, tessere[id].val1, tessere[id].col2, tessere[id].val2);
                else
                    printf("[%c%2d|%c%2d] ", tessere[id].col2, tessere[id].val2, tessere[id].col1, tessere[id].val1);
            }
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]) {
    int nTiles, rows, cols;
    int *mark;
    tessera *tiles;
    cella **board, **best;

    // carica dati
    nTiles = loadTiles(&tiles);
    mark = (int *)calloc(nTiles, sizeof(int));
    loadBoard(&board, &best, &rows, &cols, &mark);

    printf("Scacchiera in partenza\n");
    printBoard(board, tiles, rows, cols, nTiles);
    printf("\n");

    // cerca e stampa soluzione migliore
    disp(0, board, best, tiles, mark, rows, cols, nTiles);
    printf("Il massimo punteggio è %d e può essere ottenuto con la seguente configurazione:\n", maxPoints);
    printBoard(best, tiles, rows, cols, nTiles);

    // dealloca
    for (int i = 0; i < rows; i++) {
        free(board[i]);
        free(best[i]);
    }
    free(best);
    free(board);
    free(tiles);
    free(mark);

    return 0;
}
