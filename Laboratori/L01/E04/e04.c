#include <stdio.h>
#include <string.h>

#define FILENAME "corse.txt"

struct tratta {
    char codice_tratta[30];
    char partenza[30];
    char destinazione[30];
    char data[30];
    char ora_partenza[30];
    char ora_arrivo[30];
    int ritardo;
} typedef Tratta;

enum campo { codice,
             arrivo,
             partenza,
             data,
             ora_partenza };

int caricaDati(FILE *file, Tratta corse[]);
int caricaDati(FILE *file, Tratta corse[]);
void ordinaPuntatori(Tratta *V[], int len, enum campo campo);
void stampaCorse(Tratta *V[], int len);

int main(int argc, char const *argv[]) {
    Tratta corse[1000];
    char scelta;
    FILE *f = fopen(FILENAME, "r");
    int len = caricaDati(f, corse);
    fclose(f);

    // inizializza e collega matrice di puntatori
    // una riga per ogni chiave di ordimento (enum campo)
    Tratta *M[5][1000];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < len; j++) {
            M[i][j] = &corse[j];
        }
        ordinaPuntatori(M[i], len, i);
    }

    // stampa secondo l'ordinamento richiesto
    printf("Stampa la lista ordinata secondo\n\t(C) codice\n\t(A) Stazione di arrivo\n\t(P) Stazione di partenza\n\t(D) Data\n\t(O) Ora di partenza\n(Q) Quit\n");

    do {
        printf("\n> ");
        scanf(" %c", &scelta);
        switch (scelta) {
            case 'C':
                stampaCorse(M[codice], len);
                break;
            case 'A':
                stampaCorse(M[arrivo], len);
                break;
            case 'P':
                stampaCorse(M[partenza], len);
                break;
            case 'D':
                stampaCorse(M[data], len);
                break;
            case 'O':
                stampaCorse(M[ora_partenza], len);
                break;
            case 'Q':
                printf("Quit.\n");
                break;
            default:
                printf("Scelta non valida.\n");
                break;
        }
    } while (scelta != 'Q');

    return 0;
}

int caricaDati(FILE *file, Tratta corse[]) {
    int rows;

    fscanf(file, "%d\n", &rows);
    for (int i = 0; i < rows; i++) {
        fscanf(file, "%s%s%s%s%s%s%d\n", corse[i].codice_tratta, corse[i].partenza, corse[i].destinazione, corse[i].data, corse[i].ora_partenza, corse[i].ora_arrivo, &corse[i].ritardo);
    }
    return rows;
}

int strcmpCampi(Tratta *a, Tratta *b, enum campo campo) {
    // permette di usare una funzione unica ordina() per ordinare rispetto a 3 diversi campi
    switch (campo) {
        case arrivo:
            return strcmp(a->destinazione, b->destinazione);
        case partenza:
            return strcmp(a->partenza, b->partenza);
        case codice:
            return strcmp(a->codice_tratta, b->codice_tratta);
        case data:
            return strcmp(a->data, b->data);
        case ora_partenza:
            return strcmp(a->ora_partenza, b->ora_partenza);
    }
}

void ordinaPuntatori(Tratta *V[], int len, enum campo campo) {
    Tratta *x;
    int i, j;
    for (i = 1; i < len; i++) {
        x = V[i];
        j = i - 1;

        while (j >= 0 && strcmpCampi(x, V[j], campo) < 0) {
            V[j + 1] = V[j];
            j--;
        }
        V[j + 1] = x;
    }
}

void stampaCorse(Tratta *V[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%s %s %s %s %s %s\n", V[i]->codice_tratta, V[i]->partenza, V[i]->destinazione, V[i]->data, V[i]->ora_partenza, V[i]->ora_arrivo);
    }
}