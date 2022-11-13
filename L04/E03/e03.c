#include <stdio.h>
#include <stdlib.h>

#define FILENAME "easy_test_set.txt"

enum pietra { zaffiro,
              rubino,
              topazio,
              smeraldo };

// non ho una lunghezza target!!

// parto da zero a salire
// finché esiste una collana lunga k, provo k+1

// parto dalla lunghezza massima (somma di tutte le gemme) a scendere
// contro: devo esplorare spazi molto grandi

// approccio dicotomico: parto da metà, se esiste provo nella metà del
// sottovettore superiore, altrimenti di quello inferiore

// meno funzioni per non aumentare troppo il tempo di esecuzione

// controlla la validità il prima possibile!! PRUNING

// cerca una collana valida di lunghezza len e restituisce 1 se la trova
int collana(int pos, int *mark, int *sol, int len, int found) {
    int check;
    // terminazione
    if (pos == len) {
        // for (int i = 0; i < len; i++)
        //     printf("%d ", sol[i]);
        // printf("\n");

        return 1;
        // è sufficiente una soluzione!!
        // esci da tutte le ricorsioni => flag? if (found == 1) return 1;
        // vedi teoria
    }
    // itero sulle 4 pietre
    for (int i = 0; i < 4; i++) {
        // controllo che la pietra sia ancora disponibile e che possa seguire quella precedente
        switch (i) {
            case zaffiro:
                check = (sol[pos] == zaffiro || sol[pos] == topazio);
                break;
            case smeraldo:
                check = (sol[pos] == smeraldo || sol[pos] == rubino);
                break;
            case rubino:
                check = (sol[pos] == topazio || sol[pos] == zaffiro);
                break;
            case topazio:
                check = (sol[pos] == smeraldo || sol[pos] == rubino);
                break;
        }
        if (mark[i] > 0 && check) {
            sol[pos] = i;
            mark[i]--;
            found = collana(pos + 1, mark, sol, len, found);
            if (found == 1) return 1;
            mark[i]++;  // restituisco la pietra}
        }
        // else ?? provo un'altra pietra
    }
    return found;
}

int main(int argc, char const *argv[]) {
    int nTest, found, t = 0, maxLen = 0, nPietre = 0;
    int mark[4];  // tiene traccia delle pietre rimaste
    int *sol;
    FILE *f = fopen(FILENAME, "r");
    fscanf(f, "%d", &nTest);

    while (t < nTest) {
        t++;
        nPietre = 0;
        for (int i = 0; i < 4; i++) {
            fscanf(f, "%d", &mark[i]);
            nPietre += mark[i];
        }
        sol = (int *)malloc(nPietre * sizeof(int));

        printf("Test #%d\n", t);
        printf("zaffiri = %d, rubini = %d, topazi = %d, smeraldi = %d, TOT = %d\n", mark[zaffiro], mark[rubino], mark[topazio], mark[smeraldo], nPietre);

        // approccio dicotomico alla ricerca della lunghezza max
        int a = 0, b = nPietre, k;
        while (a <= b) {
            k = (a + b) / 2;
            found = collana(0, mark, sol, k, 0);
            if (found) {  // aggiorno maxLen e cerco una lunghezza maggiore
                maxLen = k;
                a = k + 1;
            } else {  // provo con una lunghezza minore
                b = k - 1;
            }
            // printf("Collane di len %d: %d\n", k, found);
        }
        printf("Lunghezza max: %d\n", maxLen);

        free(sol);  // use realloc instead??
    }
    fclose(f);
    return 0;
}
