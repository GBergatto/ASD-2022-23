#include <stdio.h>
#include <string.h>

#include "quotation.h"
#include "tempo.h"
#include "title.h"

#define FILE_NAME "F3.txt"
#define MAX_SIZE 21

int loadFile(FILE *f, TC tc) {
    t_data prevDate, date;
    t_orario time;
    Quotation x;
    Title t;
    char code[21];
    int nT, nQ, n, year, month, day, hour, min, sumN;
    float val, sumQ;

    fscanf(f, "%d", &nT);
    for (int i = 0; i < nT; i++) {
        sumN = 0, sumQ = 0;

        fscanf(f, "%s %d", code, &nQ);

        // cerca titolo letto da file
        // se non è ancora presente nella lista, aggiungilo
        t = TCsearch(tc, code);
        if (strcmp(TITLEgetName(t), "") == 0) {
            t = TITLEsetName(t, code);
            TCaddTitle(tc, t);
        }

        // ### ERRORE ###
        // La lettura e il calcolo delle quotazioni si basa (erroneamente) sull'assuzione che le
        // transazioni con la stessa data siano adiacenti e appartenenti allo stesso file.

        // Per poter modificare la quotazione giornaliera di un titolo aggiungendo transazioni in un
        // secondo momento, occorre mantenere una lista di tutte le transazioni avvenute in quella data.
        // Questo potrebbe essere fatto includendo una linked list all'interno dell'ADT Quotation, in
        // cui ogni nodo contiene l'ora della transazione, il valore e il numero. La quotazione del titolo
        // verrebbe calcolata su richiesta scorrendo le transazioni presenti in tale lista in quel momento.

        for (int j = 0; j < nQ; j++) {
            fscanf(f, "%d/%d/%d %d:%d %f %d", &(date.year), &(date.month),
                   &(date.day), &(time.hours), &(time.minutes), &val, &n);

            // se la data è diversa dalla precedente, salvo la quotazione precedente
            if ((j > 0 && DATEcmp(prevDate, date) != 0)) {
                x.date = prevDate;
                x.value = sumQ / sumN;
                BSTinsertLeaf(TITLEgetQuotations(t), x);
                sumN = 0, sumQ = 0;
            }

            sumQ += val * n;
            sumN += n;

            prevDate.day = date.day;
            prevDate.month = date.month;
            prevDate.year = date.year;
        }
        // salvo la quotazione dell'ultimo giorno dell'elenco
        x.date = date;
        x.value = sumQ / sumN;
        BSTinsertLeaf(TITLEgetQuotations(t), x);
    }
    return 0;
}

void printMenu() {
    printf("MENU\n");
    printf("1. leggi file\n");
    printf("2. seleziona titolo\n");
    printf("0. quit\n");
}

void printTitleMenu() {
    printf("1. ricerca quotazione in data\n");
    printf("2. quotazione min e max tra 2 date\n");
    printf("3. quotazione min e max totale\n");
    printf("4. bilanciamento BST con soglia S\n");
    printf("0. esci dal titolo selezionato\n");
}

int main(int argc, char const *argv[]) {
    FILE *f;
    TC tc = TCinit();

    f = fopen(FILE_NAME, "r");
    if (f == NULL) {
        printf("Errore nella lettura del file\n");
        return 1;
    }
    loadFile(f, tc);

    printMenu();

    int opt;
    do {
        printf("\n> ");
        scanf("%d", &opt);

        switch (opt) {
            case 0:
                printf("Quitting...\n");
                break;
            case 1:
                printf("Inserisci nome file: ");
                char fname[MAX_SIZE];
                scanf("%s", fname);

                f = fopen(fname, "r");
                if (f == NULL) {
                    printf("Errore nella lettura del file\n");
                }
                loadFile(f, tc);
                break;
            case 2:
                printf("Inserisci nome titolo: ");
                char tname[MAX_SIZE];
                scanf("%s", tname);
                Title title = TCsearch(tc, tname);
                if (strcmp(TITLEgetName(title), "") == 0)
                    printf("Titolo '%s' non trovato\n", tname);
                else {
                    printTitleMenu();
                    int topt;
                    do {
                        printf("\n(%s) >> ", tname);
                        scanf("%d", &topt);

                        switch (topt) {
                            case 0:
                                printf("Titolo deselezionato\n");
                                break;
                            case 1: {
                                printf("Inserisci anno: ");
                                t_data date;
                                scanf("%d", &(date.year));
                                printf("Inserisci mese: ");
                                scanf("%d", &(date.month));
                                printf("Inserisci giorno: ");
                                scanf("%d", &(date.day));

                                Quotation quote = BSTsearch(TITLEgetQuotations(title), date);
                                if (quote.date.year == 0) {
                                    printf("Nessuna quotazione nella data selezionata.\n");
                                } else {
                                    printf("Quotazione = %f\n", quote.value);
                                }
                                break;
                            }
                            case 2:  // quotazione max/min in range di date
                            {
                                t_data start, end;
                                printf("Inserisci anno di inizio: ");
                                scanf("%d", &(start.year));
                                printf("Inserisci mese di inizio: ");
                                scanf("%d", &(start.month));
                                printf("Inserisci giorno di inizio: ");
                                scanf("%d", &(start.day));
                                printf("Inserisci anno di fine: ");
                                scanf("%d", &(end.year));
                                printf("Inserisci mese di fine: ");
                                scanf("%d", &(end.month));
                                printf("Inserisci giorno di fine: ");
                                scanf("%d", &(end.day));

                                Quotation max = QsetNull(), min = QsetNull();
                                BSTfindExtremesInRange(TITLEgetQuotations(title), start, end, &max, &min);
                                if (max.date.year == 0) {
                                    printf("Nessuna quotazione nel range selezionato\n");
                                } else {
                                    printf("Quotazione massima = %f in data %04d/%02d/%02d\n", max.date.year, max.date.month, max.date.day, max.value);
                                    printf("Quotazione minima = %f in data %04d/%02d/%02d\n", min.date.year, min.date.month, min.date.day, min.value);
                                }
                                break;
                            }
                            case 3:  // quotazione max/min complessiva
                            {
                                t_data start, end;
                                // gli estremi dell'intervallo solo le date minima e massima
                                start = BSTmin(TITLEgetQuotations(title)).date;
                                end = BSTmax(TITLEgetQuotations(title)).date;
                                printf("Range %04d/%02d/%02d -> %04d/%02d/%02d\n", start.year, start.month, start.day, end.year, end.month, end.day);

                                Quotation max = QsetNull(), min = QsetNull();
                                BSTfindExtremesInRange(TITLEgetQuotations(title), start, end, &max, &min);
                                if (max.date.year == 0) {
                                    printf("Nessuna quotazione nel range selezionato\n");
                                } else {
                                    printf("Quotazione massima = %f in data %04d/%02d/%02d\n", max.date.year, max.date.month, max.date.day, max.value);
                                    printf("Quotazione minima = %f in data %04d/%02d/%02d\n", min.date.year, min.date.month, min.date.day, min.value);
                                }
                                break;
                            }
                            case 4:  // bilanciamento BST se superata soglia
                            {
                                float S, ratio;
                                printf("Inserisci soglia: ");
                                scanf("%f", &S);
                                ratio = BSTgetPathsRatio(TITLEgetQuotations(title));
                                printf("Rapporto tra cammino max e min = %f\n", ratio);
                                if (ratio > S) {
                                    BSTbalance(TITLEgetQuotations(title));
                                }
                                break;
                            }
                            default:
                                printf("Opzione non valida\n");
                                break;
                        }
                    } while (topt != 0);
                }
                break;
            default:
                printf("Opzione non valida\n");
                break;
        }

    } while (opt != 0);

    return 0;
}
