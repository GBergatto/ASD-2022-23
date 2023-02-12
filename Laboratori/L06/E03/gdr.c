#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invArray.h"
#include "pg.h"
#include "pgList.h"

#define N_SCELTE 7
#define DBG 0

enum { falso,
       vero };
typedef int bool;

void stampaMenu(char *scelte[], int *selezione) {
    int i = 0;
    printf("\nMENU'\n");
    for (i = 0; i < N_SCELTE; i++)
        printf("%2d > %s\n", i, scelte[i]);
    printf("> ");
    scanf(" %d", selezione);
}

int main(int argc, char **argv) {
    char *scelte[] = {
        "Uscita",
        "Stampa personaggi",
        "Stampa inventario",
        "Cerca personaggio",
        "Aggiungi personaggio",
        "Elimina personaggio",
        "Modifica equipaggiamento",
    };

    char codiceRicerca[LEN];
    int selezione;
    FILE *fin;
    bool fineProgramma;

    pgList personaggi = list_init();
    invArray invArray = invArray_init();
    pg *pgp, pg;

    fin = fopen("pg.txt", "r");
    pgList_read(fin, personaggi);
    fclose(fin);
#if DBG
    pgList_print(stdout, pgList);
#endif /* DBG */

    fin = fopen("inventario.txt", "r");
    invArray_read(fin, invArray);
    fclose(fin);
#if DBG
    invArray_print(stdout, invArray);
#endif /* DBG */

    fineProgramma = falso;

    do {
        stampaMenu(scelte, &selezione);
        switch (selezione) {
            case 0: {
                fineProgramma = vero;
            } break;

            case 1: {
                pgList_print(stdout, personaggi, invArray);
            } break;

            case 2: {
                invArray_print(stdout, invArray);
            } break;

            case 3: {
                printf("Inserire codice personaggio: ");
                scanf("%s", codiceRicerca);
                pgp = pgList_searchByCode(personaggi, codiceRicerca);
                if (pgp != NULL) {
                    pg_print(stdout, pgp, invArray);
                } else {
                    printf("Nessun personaggio con codice '%s'\n", codiceRicerca);
                }
            } break;

            case 4: {
                printf("Cod Nome Classe HP MP ATK DEF MAG SPR: ");
                if (pg_read(stdin, &pg) != 0) {
                    pgList_insert(personaggi, pg);
                }
            } break;

            case 5: {
                printf("Inserire codice personaggio: ");
                scanf("%s", codiceRicerca);
                pgList_remove(personaggi, codiceRicerca);
            } break;

            case 6: {
                printf("Inserire codice personaggio: ");
                scanf("%s", codiceRicerca);
                pgp = pgList_searchByCode(personaggi, codiceRicerca);
                if (pgp != NULL) {
                    pg_print(stdout, pgp, invArray);
                    char opt, nomeOggetto[51];
                    printf("\nVuoi aggiungere (a) o togliere (t) un oggetto? ");
                    scanf(" %c", &opt);
                    if (opt == 'a') {  // aggiungi
                        invArray_print(stdout, invArray);
                        printf("\nInserire il nome dell'oggetto da aggiungere: ");
                        scanf("%s", nomeOggetto);
                        // cerco tra gli oggetti nell'inventario
                        int index = invArray_searchByName(invArray, nomeOggetto);

                        if (index != -1) {
                            int res = pg_addObject(pgp, invArray, index);
                            if (res == -1)
                                printf("Impossibile aggiungere l'oggetto selezionato. Limite raggiunto.\n");
                            else
                                printf("Oggetto '%s' equipaggiato.\n", nomeOggetto);

                        } else
                            printf("Oggetto '%s' non trovato.\n");

                    } else if (opt == 't') {  // togli
                        printf("Inserire nome dell'oggetto da rimuovere: ");
                        scanf("%s", nomeOggetto);
                        // cerco tra gli oggetti equipaggiati
                        int index = equipArray_searchByName(pgp->equip, nomeOggetto);

                        if (index != -1) {
                            pg_removeObject(pgp, index);
                            printf("Oggetto '%s' rimosso.\n", nomeOggetto);
                        } else
                            printf("Oggetto '%s' non trovato.\n");

                    } else {
                        printf("Scelta '%c' non valida.\n", opt);
                    }

                } else {
                    printf("Nessun personaggio con codice '%s'\n", codiceRicerca);
                }
            } break;

            default: {
                printf("Scelta non valida\n");
            } break;
        }
    } while (!fineProgramma);

    pgList_free(personaggi);

    return 0;
}
