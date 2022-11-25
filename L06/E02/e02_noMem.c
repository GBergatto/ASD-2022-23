#include <stdio.h>
#include <stdlib.h>

#define FILENAME "hard_test_set.txt"

enum pietra { zaffiro,
              rubino,
              topazio,
              smeraldo };

// Approccio ricorsivo (Ispirazione per la soluzione con Memoization)

// Ogni collana ha lunghezza 1 + len(collana delle gemme rimanenti)
// FUNZIONA! ma è estremamente lento
// => implemento la memoization per evitare di risolvere i problemi condivisi

int max(int a, int b);
int collanaM(int *val);
int fz(int *val);
int fr(int *val);
int ft(int *val);
int fs(int *val);

int max(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    FILE *fp;
    int len, t, v[4];

    fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        printf("Errore file\n");
        return 1;
    }

    fscanf(fp, "%d\n", &t);
    for (int i = 1; i <= t; i++) {
        fscanf(fp, "%d %d %d %d", &v[zaffiro], &v[rubino], &v[topazio], &v[smeraldo]);
        len = collanaM(v);
        printf("TEST #%d\nMassima lunghezza: %d\n\n", i, len);
    }
    return 0;
}

int collanaM(int *val) {
    int maxLen;

    // calcolo la lunghezza per collane che iniziano con ciascuna pietra
    maxLen = fz(val);
    maxLen = max(maxLen, fr(val));
    maxLen = max(maxLen, ft(val));
    maxLen = max(maxLen, fs(val));

    return maxLen;
}

int fz(int *val) {
    int len1, len2;
    if (val[zaffiro] == 0) return 0;

    val[zaffiro]--;  // aggiungo uno zaffiro alla collana (ragione del +1)
    // provo ad aggiungere altre pietre a seguire sapendo che uno zaffiro
    // può essere seguito da zaffiro o rubino
    len1 = fz(val);
    len2 = fr(val);
    val[zaffiro]++;

    return 1 + max(len1, len2);  // 1 + len(collana porzione della collana a seguire)
}

int fr(int *val) {
    int len1, len2;
    if (val[rubino] == 0) return 0;

    val[rubino]--;  // aggiungo un rubino alla collana
    // può essere seguito da smeraldo o topazio
    len1 = fs(val);
    len2 = ft(val);
    val[rubino]++;

    return 1 + max(len1, len2);
}

int ft(int *val) {
    int len1, len2;
    if (val[topazio] == 0) return 0;

    val[topazio]--;  // aggiungo un topazio alla collana
    // può essere seguito da zaffiro o rubino
    len1 = fz(val);
    len2 = fr(val);
    val[topazio]++;

    return 1 + max(len1, len2);
}

int fs(int *val) {
    int len1, len2;
    if (val[smeraldo] == 0) return 0;

    val[smeraldo]--;  // aggiungo uno smeraldo alla collana
    // può essere seguito da smeraldo o topazio
    len1 = fs(val);
    len2 = ft(val);
    val[smeraldo]++;

    return 1 + max(len1, len2);
}
