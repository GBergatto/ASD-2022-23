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
void stampaCorse(Tratta corse[], int len);
void ordina(Tratta corse[], int len, enum campo campo);
void trovaCorsaBinary(Tratta corse[], int len, char p[]);
void trovaCorsaLinear(Tratta corse[], int len, char p[]);

int main(int argc, char const *argv[]) {
    Tratta corse[1000];
    char scelta;
    FILE *f = fopen(FILENAME, "r");
    int len = caricaDati(f, corse);
    fclose(f);

    printf("Menu\n(S) Stampa log\n(D) Ordinamento per data\n(C) Ordinamento per codice\n(A) Ordinamento per stazione di arrivo\n");
    printf("(P) Ordinamento per stazione di partenza\n(R) Ricerca tratta per stazione di partenza\n(Q) Quit\n");

    do {
        printf("\n> ");
        scanf(" %c", &scelta);
        switch (scelta) {
            case 'S':
                stampaCorse(corse, len);
                break;
            case 'D':
                // siccome ordina() usa algoritmo stabile, ordino prima sull'ora poi sulla data
                ordina(corse, len, ora_partenza);
                ordina(corse, len, data);
                printf("Vettore ordinato per data.\n");
                break;
            case 'C':
                ordina(corse, len, codice);
                printf("Vettore ordinato per codice.\n");
                break;
            case 'A':
                ordina(corse, len, arrivo);
                printf("Vettore ordinato per stazione di arrivo.\n");
                break;
            case 'P':
                ordina(corse, len, partenza);
                printf("Vettore ordinato per stazione di partenza.\n");
                break;
            case 'R':
                char s[30];
                printf("Stazione di partenza: ");
                scanf("%s", s);

                printf("# Ricerca lineare\n");
                trovaCorsaLinear(corse, len, s);
                printf("# Ricerca dicotomica\n");
                trovaCorsaBinary(corse, len, s);
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

void stampaCorse(Tratta V[], int len) {
    for (int i = 0; i < len; i++) {
        printf("%s %s %s %s %s %s\n", V[i].codice_tratta, V[i].partenza, V[i].destinazione, V[i].data, V[i].ora_partenza, V[i].ora_arrivo);
    }
}

int strcmpCampi(Tratta a, Tratta b, enum campo campo) {
    // permette di usare una funzione unica ordina() per ordinare rispetto a 3 diversi campi
    switch (campo) {
        case arrivo:
            return strcmp(a.destinazione, b.destinazione);
        case partenza:
            return strcmp(a.partenza, b.partenza);
        case codice:
            return strcmp(a.codice_tratta, b.codice_tratta);
        case data:
            return strcmp(a.data, b.data);
        case ora_partenza:
            return strcmp(a.ora_partenza, b.ora_partenza);
    }
}

void ordina(Tratta V[], int len, enum campo campo) {
    Tratta x;
    int i, j;
    // Insertion sort
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

void trovaCorsaBinary(Tratta V[], int len, char p[]) {
    // implementa ricerca dicotomica
    ordina(V, len, partenza);
    int left = 0, right = len - 1;
    int mid, found = 0, index;
    while (right >= left) {
        mid = left + (right - left) / 2;  // centro del vettore = estremo sx + mezza lunghezza
        if (strcmp(V[mid].partenza, p) == 0) {
            found = 1;
            index = mid;
            break;
        } else if (strcmp(V[mid].partenza, p) < 0) {
            // p è dopo la metà -> scegli sottovettore dx
            left = mid + 1;
        } else {
            // p è dopo la metà -> scegli sottovettore sx
            right = mid - 1;
        }
    }

    if (found != 1) {  // not found
        printf("Nessuna corsa con stazione di partenza '%s'\n", p);
    } else {
        // controlla intorno all'indice per occorrenze multiple
        int i = 0;
        while (index - i >= 0 && strcmp(V[index - i].partenza, p) == 0) {
            printf("%s %s %s %s %s %s\n", V[index - i].codice_tratta, V[index - i].partenza, V[index - i].destinazione, V[index - i].data, V[index - i].ora_partenza, V[index - i].ora_arrivo);
            i++;
        }
        i = 1;
        while (index + i < len && strcmp(V[index + i].partenza, p) == 0) {
            printf("%s %s %s %s %s %s\n", V[index + i].codice_tratta, V[index + i].partenza, V[index + i].destinazione, V[index + i].data, V[index + i].ora_partenza, V[index + i].ora_arrivo);
            i++;
        }
    }
}

void trovaCorsaLinear(Tratta V[], int len, char p[]) {
    // implementa ricerca lineare
    int found = 0;
    for (int i = 0; i < len; i++) {
        if (strcmp(V[i].partenza, p) == 0) {
            found++;
            printf("%s %s %s %s %s %s\n", V[i].codice_tratta, V[i].partenza, V[i].destinazione, V[i].data, V[i].ora_partenza, V[i].ora_arrivo);
        }
    }
    if (found == 0) {
        printf("Nessuna corsa con stazione di partenza '%s'\n", p);
    }
}