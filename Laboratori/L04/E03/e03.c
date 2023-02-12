#include <stdio.h>
#include <stdlib.h>

#define FILENAME "easy_test_set.txt"

enum pietra { zaffiro,
              rubino,
              topazio,
              smeraldo };

// cerca una collana valida di lunghezza len e restituisce 1 se la trova
int collana(int pos, int *mark, int *sol, int *best, int len, int found) {
    int check;
    if (pos == len) {
        // aggiorno soluzione migliore
        for (int i = 0; i < pos; i++)
            best[i] = sol[i];

        return 1;  // flag di successo: basta una soluzione
    }
    // itero sulle 4 pietre
    for (int i = 0; i < 4; i++) {
        if (pos > 0) {  // controllo che la pietra possa seguire la precedente (se non è la prima)
            switch (i) {
                case zaffiro:
                    check = (sol[pos - 1] == zaffiro || sol[pos - 1] == topazio);
                    break;
                case smeraldo:
                    check = (sol[pos - 1] == smeraldo || sol[pos - 1] == rubino);
                    break;
                case rubino:
                    check = (sol[pos - 1] == topazio || sol[pos - 1] == zaffiro);
                    break;
                case topazio:
                    check = (sol[pos - 1] == smeraldo || sol[pos - 1] == rubino);
                    break;
            }
        }
        if (mark[i] > 0 && check) {  // la pietra è presente e valida
            sol[pos] = i;
            mark[i]--;
            found = collana(pos + 1, mark, sol, best, len, found);
            mark[i]++;  // restituisco la pietra
            if (found == 1) return 1;
        }
    }
    return found;
}

int main(int argc, char const *argv[]) {
    int nTest, found, t = 0, maxLen = 0, nPietre = 0;
    int mark[4];  // tiene traccia delle pietre rimaste
    int *sol, *best;
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
        best = (int *)malloc(nPietre * sizeof(int));

        // approccio dicotomico alla ricerca della lunghezza max
        int a = 0, b = nPietre, k;
        while (a <= b) {
            k = (a + b) / 2;
            found = collana(0, mark, sol, best, k, 0);
            if (found) {  // aggiorno maxLen e cerco una lunghezza maggiore
                maxLen = k;
                a = k + 1;
            } else {  // provo con una lunghezza minore
                b = k - 1;
            }
        }

        printf("#%d Lunghezza max: %d\n", t, maxLen);
        char intToChar[4] = {'z', 't', 'r', 's'};
        for (int i = 0; i < maxLen; i++)
            printf("%c ", intToChar[best[i]]);
        printf("\n\n");

        free(sol);
    }
    fclose(f);
    return 0;
}
