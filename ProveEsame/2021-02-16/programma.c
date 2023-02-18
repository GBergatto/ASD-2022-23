#include <string.h>
#include <stdlib.h>

#include "programma.h"
#include "articolo.h"
#include "insieme.h"

struct programma {
    int R; // numero di stanze
    int S; // numero di slot
    char ***mat; // matrice di stringhe
};

PROGRAMMA PROGload(FILE *f, int R, int S) {
    PROGRAMMA p = PROGinit(R, S);

    for (int i=0; i<p->R; i++) {
        for (int j=0; j<p->S; j++) {
            fscanf(f, "%s", p->mat[i][j]);
        }
    }
    return p;
}

int PROGcheck(PROGRAMMA p, ARTICOLI st) {
    int cnt;
    char title[MAXLEN], prec[MAXLEN];
    ARTICOLO a;
    Set titoli, autori;

    // controllo che ogni articolo abbia abbastanza slot a disposizione
    for (int i=0; i<p->R; i++) {
        cnt = 1;

        strcpy(prec, p->mat[i][0]);
        for (int j=1; j<p->S; j++) {
            strcpy(title, p->mat[i][j]);
            if (strcmp(title, prec) == 0 && j != p->S) {
                cnt++;
            } else {
                if (strcmp(prec, "-") != 0) {
                    a = ARTICOLIsearch(st, prec);
                    if (ARTICOLOisVoid(a))
                        return 0; // articolo non trovato
                    else if (a.slot != cnt)
                            return 0; // numero di slot sbagliato
                }
                cnt = 1;
            }
            strcpy(prec, title);
        }
    }
    
    // controllo presenta dello stesso autore in stanze diverse
    // per ogni slot, tanti relatori quante sono le stanze con articolo

    autori = SETinit(p->S); // insieme di autori (stringhe)
    cnt = 0; // articoli non nulli per slot
    for (int slot=0; slot < p->S; slot++) {
        for (int r=0; r < p->R; r++) {
            if (strcmp(p->mat[r][slot], "-") != 0) {
                cnt++;
                a = ARTICOLIsearch(st, p->mat[r][slot]);
                SETfill(autori, a.relatore);
                SETfill(titoli, a.titolo);
            }
        }
        if (SETsize(autori) != cnt) // numero di relatori distinti
            return 0;
        SETdeleteAll(autori);
    }
    if (SETsize(titoli) != ARTICOLIsize(st)) // numero di articoli
        return 0;

    SETfree(titoli);
    SETfree(autori);
    return 1;
}

PROGRAMMA PROGgenerateBest(ARTICOLI st, int R, int S) {
    PROGRAMMA sol = PROGinit(R, S);
    PROGRAMMA best = PROGinit(R, S);
    int *mark = malloc(ARTICOLIsize(st)*sizeof(int));
    int score;

    disp(0, st, mark, sol, best, &score);

    return best;
}


void disp(int pos, ARTICOLI st, int *mark, PROGRAMMA sol, PROGRAMMA best, int *bScore) {
    ARTICOLO a;

    if (pos >= sol->R*sol->S) {
        if (PROGcheck(sol, st)) { // valido
            int score = PROGgetScore(sol, st);
            if (score < *bScore) {
                PROGcpy(best, sol);
                *bScore = score;
            }
        }
        return;
    }

    for (int i=0; i<ARTICOLIsize(st); i++) {
        if (mark[i] == 0) {
            a = ARTICOLIsearchByIndex(st, i);
            if (sol->S - pos/sol->S >= a.slot) { // se ci sono abbastanza slot per l'articolo
                mark[i] = 1;
                for (int j= pos/sol->S; j < sol->S; j++)
                    sol->mat[pos%sol->S][j] = a.titolo;

                disp(pos+a.slot, st, mark, sol, best, bScore);
                mark[i] = 0; // smarcamento
            }
        }
    }
    // oppure ricorri lasciando lo slot corrente vuoto
    sol->mat[pos%sol->S][pos/sol->S] = "-";
    disp(pos+1, st, mark, sol, best, bScore);
}

int PROGgetScore(PROGRAMMA p, ARTICOLI st) {
    int last, nv = 0, na = 0;
    ARTICOLO a;
    Set s = SETinit(p->S);

    for (int i=0; i < p->R; i++) {
        last = p->S-1;
        // cerco l'ultimo slot pieno
        while (last>0 && strcmp(p->mat[i][last], "-")) last--;

        for (int j=0; j < last; j++) {
            if (strcmp(p->mat[i][j], "-")) // slot vuoto in mezzo alla giornata
                nv++;
            else {
                    a = ARTICOLIsearch(st, p->mat[i][j]);
                    SETfill(s, a.argomento);
                }
        }
        na += SETsize(s); // numero di argomenti distinti per stanza
        SETdeleteAll(s);
    }

    SETfree(s);
    return nv+na;
}
