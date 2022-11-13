#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 51
#define DATELEN 11
#define OUT_FILE "output.txt"

typedef struct {
    char codice[6];
    char nome[MAXLEN];
    char cognome[MAXLEN];
    char data[DATELEN];
    char via[MAXLEN];
    char citta[MAXLEN];
    int cap;
} Item;

typedef struct node *link;
struct node {
    Item val;
    link next;
};

// scompongo la data in giorno, mese e anno per ordinamento
// minima complessità funzionale:
// ogni funzione deve fare il minimo indispensabile
// maggiore modularità, funzioni più generali

link newNode(Item val, link next);
link sortedInsert(link h, Item val);
int datecmp(char a[], char b[]);

Item emptyItem() {
    Item empty;
    strcpy(empty.codice, "00000");
    return empty;
}

link newNode(Item val, link next) {
    link x = malloc(sizeof *x);
    if (x == NULL)  // errore di allocazione
        return NULL;
    else {
        x->val = val;
        x->next = next;
    }
    return x;
}

/// @brief Compare date in DD/MM/YYYY format
/// @param a
/// @param b
/// @return >0 if a comes after b, <0 if a comes before b, 0 if a equals b
int datecmp(char a[], char b[]) {
    int anno = strcmp(&a[6], &b[6]);
    if (anno != 0)
        return anno;
    else {  // stesso anno
        int mese = strcmp(&a[3], &b[3]);
        if (mese != 0)
            return mese;
        else  // stesso mese
            return strcmp(a, b);
    }
};

link sortedInsert(link h, Item val) {
    link x, p;
    if (h == NULL || datecmp(val.data, h->val.data) < 0)
        return newNode(val, h);  // aggiungi in testa

    int c;

    // scorro la lista finché la data da aggiungere è successiva a dell'elemento puntato
    for (x = h->next, p = h; x != NULL && datecmp(val.data, x->val.data) > 0; p = x, x = x->next)
        c = datecmp(val.data, h->val.data);

    // ho raggiunto la fine della lista oppure ho trovato un nodo con data successiva a quella da aggiungere
    p->next = newNode(val, x);
    return h;  // head resta invariato
}

Item search(link h, char codice[6]) {
    Item empty = emptyItem();

    for (link x = h; x != NULL; x = x->next) {
        if (strcmp(codice, x->val.codice) == 0)
            return x->val;
    }
    return empty;  // no match found
}

Item estraiDaCodice(link *hp, char codice[]) {
    link *xp, t;
    Item i = emptyItem();

    for (xp = hp; (*xp) != NULL; xp = &((*xp)->next)) {
        if (strcmp((*xp)->val.codice, codice) == 0) {
            t = *xp;  // puntatore temporaneo al nodo da estrarre
            // aggiorno il puntatore (potenzialmente h) del nodo precedente a quello estratto
            *xp = (*xp)->next;
            i = t->val;
            free(t);
            break;
        }
    }
    return i;
}

Item estraiDaDate(link *hp, char start[DATELEN], char end[DATELEN]) {
    link *xp, t;
    Item i = emptyItem();

    for (xp = hp; (*xp) != NULL; xp = &((*xp)->next)) {
        // data del nodo compresa tra start e end
        if (datecmp((*xp)->val.data, start) >= 0 && datecmp((*xp)->val.data, end) <= 0) {
            t = *xp;  // puntatore temporaneo al nodo da estrarre
            // aggiorno il puntatore (potenzialmente h) del nodo precedente a quello estratto
            *xp = (*xp)->next;
            i = t->val;
            free(t);
            break;
        }
    }
    return i;
}

int stampa(link h) {
    FILE *f = fopen(OUT_FILE, "w");
    if (f == NULL) {
        return -1;
    }

    int cnt = 0;
    for (link x = h; x != NULL; x = x->next) {
        fprintf(f, "%s %s %s %s %s %s %d\n", x->val.codice, x->val.nome, x->val.cognome, x->val.data, x->val.via, x->val.citta, x->val.cap);
        cnt++;
    }
    fclose(f);
    return cnt;
}

int main(int argc, char const *argv[]) {
    printf("Opzioni\n");
    printf("1 - Nuovo elemento da tastiera\n");
    printf("2 - Nuovo elemento da file\n");
    printf("3 - Ricerca elemento per codice\n");
    printf("4 - Elimina elemento per codice\n");
    printf("5 - Elimina elementi in range di date\n");
    printf("6 - Stampa lista su file\n");
    printf("0 - Quit\n");

    link head = NULL;
    int c;
    Item item;
    char code[6];

    do {
        do {
            printf("> ");
            scanf("%d", &c);
        } while (c < 0 || c > 6);

        switch (c) {
            case 1:  // nuovo elemento da tastiera
                printf("Inserisci elemento: ");
                scanf("%s %s %s %s %s %s %d",
                      item.codice,
                      item.nome,
                      item.cognome,
                      item.data,
                      item.via,
                      item.citta,
                      &(item.cap));
                head = sortedInsert(head, item);
                printf("Elemento inserito.\n");
                break;
            case 2:  // nuovo elemento da file
                char fname[100];
                printf("Inserisci nome file: ");
                scanf("%s", fname);
                FILE *f = fopen(fname, "r");
                if (f == NULL) {
                    printf("Errore nell'apertura del file '%s'\n", fname);
                    break;
                }
                // leggo il file linea per linea
                int cnt = 0;
                while (fscanf(f, "%s %s %s %s %s %s %d", item.codice, item.nome, item.cognome, item.data, item.via, item.citta, &(item.cap)) == 7) {
                    head = sortedInsert(head, item);
                    cnt++;
                }
                printf("Inseriti %d elementi dal file '%s'\n", cnt, fname);
                break;
            case 3:  // ricerca per codice
                printf("Inserisci codice: ");
                scanf("%s", code);
                item = search(head, code);
                if (strcmp(item.codice, "00000") == 0)
                    printf("Nessun elemento con codice %s\n", code);
                else
                    printf("%s %s %s %s %s %s %d\n", item.codice, item.nome, item.cognome, item.data, item.via, item.citta, item.cap);
                break;
            case 4:  // elimina per codice
                printf("Inserisci codice: ");
                scanf("%s", code);
                item = estraiDaCodice(&head, code);
                if (strcmp(item.codice, "00000") == 0)
                    printf("Nessun elemento con codice %s\n", code);
                else {
                    printf("Estratto il seguente elemento dalla lista:\n");
                    printf("%s %s %s %s %s %s %d\n", item.codice, item.nome, item.cognome, item.data, item.via, item.citta, item.cap);
                }
                break;
            case 5:  // elimina range di date
                char start[DATELEN], end[DATELEN];
                printf("Inserisci data di inizio: ");
                scanf("%s", start);
                printf("Inserisci data di fine: ");
                scanf("%s", end);
                while (1) {  // finche trovo un nodo con data nel range lo estraggo e riprovo
                    item = estraiDaDate(&head, start, end);
                    printf("Estratto i elementi dalla lista:\n");
                    if (strcmp(item.codice, "00000") != 0) {
                        printf("%s %s %s %s %s %s %d\n", item.codice, item.nome, item.cognome, item.data, item.via, item.citta, item.cap);
                    } else
                        break;
                }

                break;
            case 6:  // stampa su file
                int len = stampa(head);
                if (len == -1)
                    printf("Errore nella creazione del file di output");
                else if (len == 0)
                    printf("La lista è vuota\n");
                else
                    printf("Lista stampata su %s\n", OUT_FILE);
                break;
        }
    } while (c != 0);

    printf("Quit.\n");
    return 0;
}
