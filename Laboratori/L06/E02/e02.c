#include <stdio.h>
#include <stdlib.h>

#define FILENAME "easy_test_set.txt"

enum pietra { zaffiro,
              rubino,
              topazio,
              smeraldo };

// Memoization
// Matrice a 4 dimensioni per memoizzare i risultati delle funzioni ricorsive
// memX[z][r][t][s] = lunghezza ottenuta lasciando fuori z zaffini, r rubini, t topazi e s smeraldi
// Una matrice per ognuna delle 4 funzioni fz, fr, ft, fs

int max(int a, int b);
int collanaM(int *val);
int ****malloc4d(int nZ, int nR, int nT, int nS);
void freeMemoization(int ****memZ, int ****memR, int ****memT, int ****memS, int nz, int nr, int nt);
int fz(int ****mz, int ****mr, int ****mt, int ****ms, int *val);
int fr(int ****mz, int ****mr, int ****mt, int ****ms, int *val);
int ft(int ****mz, int ****mr, int ****mt, int ****ms, int *val);
int fs(int ****mz, int ****mr, int ****mt, int ****ms, int *val);

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
    int ****memZ, ****memR, ****memT, ****memS, maxLen;
    int nz = val[zaffiro], nr = val[rubino], nt = val[topazio], ns = val[smeraldo];

    // alloco le matrici per la memoization
    memZ = malloc4d(nz, nr, nt, ns);
    memR = malloc4d(nz, nr, nt, ns);
    memT = malloc4d(nz, nr, nt, ns);
    memS = malloc4d(nz, nr, nt, ns);

    // calcolo la lunghezza per collane che iniziano con ciascuna pietra
    maxLen = fz(memZ, memR, memT, memS, val);
    maxLen = max(maxLen, fr(memZ, memR, memT, memS, val));
    maxLen = max(maxLen, ft(memZ, memR, memT, memS, val));
    maxLen = max(maxLen, fs(memZ, memR, memT, memS, val));

    // dealloco e resistituisco la lunghezza massima trovata
    freeMemoization(memZ, memR, memT, memS, nz, nr, nt);
    return maxLen;
}

int ****malloc4d(int nZ, int nR, int nT, int nS) {
    int ****mat;
    mat = (int ****)malloc((nZ + 1) * sizeof(int ***));
    for (int z = 0; z <= nZ; z++) {
        mat[z] = (int ***)malloc((nR + 1) * sizeof(int **));
        for (int r = 0; r <= nR; r++) {
            mat[z][r] = (int **)malloc((nT + 1) * sizeof(int *));
            for (int t = 0; t <= nT; t++) {
                mat[z][r][t] = (int *)malloc((nS + 1) * sizeof(int));
                for (int s = 0; s <= nS; s++)
                    mat[z][r][t][s] = -1;
            }
        }
    }
    return mat;
}

void freeMemoization(int ****memZ, int ****memR, int ****memT, int ****memS, int nz, int nr, int nt) {
    int i, j, k;
    for (i = 0; i <= nz; i++) {
        for (j = 0; j <= nr; j++) {
            for (k = 0; k <= nt; k++) {
                free(memZ[i][j][k]);
                free(memR[i][j][k]);
                free(memT[i][j][k]);
                free(memS[i][j][k]);
            }
            free(memZ[i][j]);
            free(memR[i][j]);
            free(memT[i][j]);
            free(memS[i][j]);
        }
        free(memZ[i]);
        free(memR[i]);
        free(memT[i]);
        free(memS[i]);
    }
    free(memZ);
    free(memR);
    free(memT);
    free(memS);
}

int fz(int ****mz, int ****mr, int ****mt, int ****ms, int *val) {
    int len1, len2, out;
    if (val[zaffiro] == 0) return 0;
    // memoization
    if (mz[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] != -1)
        return mz[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]];

    val[zaffiro]--;  // aggiungo uno zaffiro alla collana (ragione del +1 alla lunghezza max)
    // provo ad aggiungere altre pietre a seguire sapendo che uno zaffiro
    // può essere seguito da zaffiro o rubino
    len1 = fz(mz, mr, mt, ms, val);
    len2 = fr(mz, mr, mt, ms, val);
    val[zaffiro]++;
    out = 1 + max(len1, len2);  // 1 + len(porzione della collana a seguire)
    // salvo la lunghezza appena calcolata
    mz[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] = out;

    return out;
}

int fr(int ****mz, int ****mr, int ****mt, int ****ms, int *val) {
    int len1, len2, out;
    if (val[rubino] == 0) return 0;
    // memoization
    if (mr[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] != -1)
        return mr[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]];

    val[rubino]--;  // aggiungo un rubino alla collana
    // può essere seguito da smeraldo o topazio
    len1 = fs(mz, mr, mt, ms, val);
    len2 = ft(mz, mr, mt, ms, val);
    val[rubino]++;
    out = 1 + max(len1, len2);
    // salvo la lunghezza appena calcolata
    mr[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] = out;

    return out;
}

int ft(int ****mz, int ****mr, int ****mt, int ****ms, int *val) {
    int len1, len2, out;
    if (val[topazio] == 0) return 0;
    // memoization
    if (mt[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] != -1)
        return mt[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]];

    val[topazio]--;  // aggiungo un topazio alla collana
    // può essere seguito da zaffiro o rubino
    len1 = fz(mz, mr, mt, ms, val);
    len2 = fr(mz, mr, mt, ms, val);
    val[topazio]++;
    out = 1 + max(len1, len2);
    // salvo la lunghezza appena calcolata
    mt[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] = out;

    return out;
}

int fs(int ****mz, int ****mr, int ****mt, int ****ms, int *val) {
    int len1, len2, out;
    if (val[smeraldo] == 0) return 0;
    // memoization
    if (ms[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] != -1)
        return ms[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]];

    val[smeraldo]--;  // aggiungo uno smeraldo alla collana
    // può essere seguito da smeraldo o topazio
    len1 = fs(mz, mr, mt, ms, val);
    len2 = ft(mz, mr, mt, ms, val);
    val[smeraldo]++;
    out = 1 + max(len1, len2);
    // salvo la lunghezza appena calcolata
    ms[val[zaffiro]][val[rubino]][val[topazio]][val[smeraldo]] = out;

    return out;
}
