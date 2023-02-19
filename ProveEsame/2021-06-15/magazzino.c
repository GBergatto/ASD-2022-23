#include <string.h>
#include <stdlib.h>

#include "magazzino.h"
#include "corridoio.h"
#include "pacco.h"
#include "scaffale.h"

struct s_magazzino {
    Corridoio *corridoi;
    int C;
    int P, maxP; // numero effettivo e max di pacchi
    Pacco *pacchi;
};

static Pacco binarySearchR(Pacco *arr, int l, int r, char *key) {
    if (l > r)
        return PACCOsetVoid();

    int m = (l+r)/2;
    if (strcmp(arr[m].codice, key) == 0)
        return arr[m];
    else if (strcmp(arr[m].codice, key) > 0)
        return binarySearchR(arr, l, m-1, key);
    else
        return binarySearchR(arr, m+1, r, key);
}

Pacco MAGAZZINOcercaPacco(Magazzino m, char *codice) {
    return binarySearchR(m->pacchi, 0, m->P-1, codice);
}

static int sortedInsert(Pacco *arr, int n, Pacco p) {
    int i = 0;
    // trovo il primo elemento maggiore di p
    while (i<n && strcmp(p.codice, arr[i].codice)>0) i++;
    for (int j = i; j<n; j++) // traslo tutti gli elementi successivi
        arr[j+1] = arr[j];
    arr[i] = p;

    return i; // indice a cui ho inserito p
}

int MAGAZZINOinserisciPacco(Magazzino m, Pacco p) {
    int id, res;

    if (p.corridoio < 0 || p.corridoio >= m->C) return -1;
    res = CORRIDOIOcheckPos(m->corridoi[p.corridoio], p.scaffale, p.slot);
    if (res != -1) return res; // posizione occupata o inesistente

    if (m->P + 1 >= m->maxP) { // controllo capienza del vettore
        m->maxP *= 2;
        m->pacchi = realloc(m->pacchi, m->maxP);
    }
    id = sortedInsert(m->pacchi, m->P, p);
    m->P++;

    CORRIDOIOinserisciPacco(m->corridoi[p.corridoio], p.scaffale, p.slot, id);

    return 1;
}

Pacco MAGAZZINOestraiPacco(Magazzino m, int corr, int scaf, int slot) {
    Pacco p;
    int id, res;

    if (corr < 0 || corr >= m->C) return PACCOsetVoid();
    res = CORRIDOIOcheckPos(m->corridoi[corr], scaf, slot);
    if (res < 0) return PACCOsetVoid(); // posizione vuota o inesistente

    id = CORRIDOIOestraiPacco(m->corridoi[corr], scaf, slot);
    p = m->pacchi[id];

    // TODO: elimina il pacco da m->pacchi facendo scorrere i successivi
    m->P--;

    return p;
}

int MAGAZZINOspostaPacco(Magazzino m, int c1, int sc1, int sl1, int c2, int sc2, int sl2) {
    Pacco p = MAGAZZINOestraiPacco(m, c1, sc1, sl1);
    if (PACCOisVoid(p))
        return -1; // nessun pacco nella posizione data

    return MAGAZZINOinserisciPacco(m, p);
}

int MAGAZZINOcompattaScaffali(Magazzino m, int corr, int target, int source) {
    int id;

    // controllo validità della posizione
    if (corr < 0 || corr >= m->C) return -1;
    id = CORRIDOIOcheckPos(m->corridoi[corr], target, 0);
    if (id == -2) return -2; // pos inesistente
    id = CORRIDOIOcheckPos(m->corridoi[corr], source, 0);
    if (id == -2) return -2; // pos inesistente

    id = CORRIDOIOcompattaScaffali(m->corridoi[corr], target, source);
    if (id == -1) return -1; // non si possono compattare

    // scorro lo scaffale target e aggiorno la posizione di tutti i pacchi
    for (int i=0; i<K; i++) {
        id = CORRIDOIOcheckPos(m->corridoi[corr], target, i);
        if (id >= 0) {
            m->pacchi[id].scaffale = target;
            m->pacchi[id].slot = i;
        }
    }

    return 1;
}
