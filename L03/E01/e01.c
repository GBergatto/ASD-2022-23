#include <stdio.h>

int majority(int *a, int N);
int recursiveMajority(int *a, int l, int r);
int getFreqency(int *a, int l, int r, int target);

int main(int argc, char const *argv[]) {
    int n = 8;
    int V[30] = {3, 3, 3, 3, 0, 0, 3, 0};

    int res = majority(V, n);
    if (res > -1)
        printf("Maggioritario: %d\n", res);
    else
        printf("Non esiste maggioritario\n");

    return 0;
}

// wrapper
int majority(int *a, int N) {
    return recursiveMajority(a, 0, N - 1);
}

int recursiveMajority(int *a, int l, int r) {
    int mid, lMaj, rMaj, lFreq, rFreq;

    // terminazione
    if (l == r) {
        return a[r];
    }
    // divisione
    mid = (l + r) / 2;
    lMaj = recursiveMajority(a, l, mid);
    rMaj = recursiveMajority(a, mid + 1, r);

    // se un numero è maggioritario di due sottovettori, allora lo è anche della loro unione
    if (lMaj == rMaj)
        return lMaj;

    // se il maggioritario di un sottovettore compare nell'unione dei due sottovettori
    // almeno N/2+1 volte, allora è maggioritario anche dell'unione
    lFreq = getFreqency(a, l, r, lMaj);
    if (lFreq > (r - l + 1) / 2) {
        return lMaj;
    }
    rFreq = getFreqency(a, l, r, rMaj);
    if (rFreq > (r - l + 1) / 2) {
        return rMaj;
    }

    return -1;
}

int getFreqency(int *a, int l, int r, int target) {
    int freq = 0;
    for (int i = l; i <= r; i++)
        if (*(a + i) == target)
            freq++;
    return freq;
}
