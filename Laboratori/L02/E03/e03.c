#include <stdio.h>
#include <stdlib.h>
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

int caricaDati(char fName[], Tratta *pCorse[], Tratta ****pM);
void ordinaPuntatori(Tratta *V[], int len, enum campo campo);
void stampaCorse(Tratta *V[], int len);

int main(int argc, char const *argv[]) {
    Tratta *corse = NULL, ***M = NULL;
    char scelta, fileName[100];
    int len;

    len = caricaDati(FILENAME, &corse, &M);

    // stampa secondo l'ordinamento richiesto
    printf("Stampa la lista ordinata secondo\n\t(C) codice\n\t(A) Stazione di arrivo\n\t(P) Stazione di partenza\n\t(D) Data\n\t(O) Ora di partenza\n(L) Leggi nuovo file\n(Q) Quit\n");

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
            case 'L':
                // leggi nuovo file
                printf("Inserisci nome file: ");
                scanf("%s", fileName);
                printf("Leggo %s\n", fileName);
                len = caricaDati(fileName, &corse, &M);
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

int caricaDati(char fName[], Tratta *pCorse[], Tratta ****pM) {
    int len;
    Tratta *corse;
    Tratta ***M;

    // leggo file
    FILE *file = fopen(fName, "r");
    if (file == NULL) {
        printf("Errore leggendo il file '%s'. La lista non è stata aggiornata.", fName);
        return len;
    }

    // libero memoria
    if (pCorse != NULL) {
        free(*pCorse);
        free(*pM);
    }

    // alloco e inizializzo vettore di tratte
    fscanf(file, "%d\n", &len);
    corse = (Tratta *)malloc(len * sizeof(Tratta));
    for (int i = 0; i < len; i++) {
        fscanf(file, "%s%s%s%s%s%s%d\n", corse[i].codice_tratta, corse[i].partenza, corse[i].destinazione, corse[i].data, corse[i].ora_partenza, corse[i].ora_arrivo, &corse[i].ritardo);
    }
    *pCorse = corse;  // aggiorno il vettore del chiamante via puntatore
    fclose(file);

    // matrice di vettori di puntatori (5 righe, una per ordinamento)
    M = (Tratta ***)malloc(5 * sizeof(Tratta ***));
    for (int i = 0; i < 5; i++) {
        // vettori di puntatori
        M[i] = (Tratta **)malloc(len * sizeof(Tratta **));
        for (int j = 0; j < len; j++) {
            M[i][j] = &corse[j];
        }
        ordinaPuntatori(M[i], len, i);
    }

    *pM = M;
    return len;
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