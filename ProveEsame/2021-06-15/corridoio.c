#include <stdlib.h>

#include "corridoio.h"
#include "scaffale.h"

struct s_corridoio {
    int S;
    Scaffale *scaffali;
};

Corridoio CORRIDOIOinit(int s) {
    Corridoio c;
    c->S = s;
    c->scaffali = malloc(s * sizeof(Scaffale));
    for (int i=0; i<s; i++)
        for (int j=0; j<K; j++)
            c->scaffali[i].slots[j] = EMPTY;
    return c;
}

int CORRIDOIOcheckPos(Corridoio c, int scaf, int slot) {
    if (scaf < 0 || scaf >= c->S || slot < 0 || slot >= K)
        return -1; // posizione inesistente
    return (c->scaffali[scaf].slots[slot] == EMPTY);
}

void CORRIDOIOinserisciPacco(Corridoio c, int scaf, int slot, int id) {
    c->scaffali[scaf].slots[slot] = id;
}

int CORRIDOIOestraiPacco(Corridoio c, int scaf, int slot) {
    return c->scaffali[scaf].slots[slot];
}

int CORRIDOIOcompattaScaffali(Corridoio c, int target, int source) {
    int free = 0, toMove = 0, s, t;

    for (int i = 0; i < K; i++) {
        if (c->scaffali[source].slots[i] != EMPTY) toMove++;
        if (c->scaffali[target].slots[i] == EMPTY) free++;
    }
    if (toMove > free) return -1; // lo scaffale target non ha spazio

    s = t = 0;
    while(toMove > 0) {
        while (c->scaffali[target].slots[t] != EMPTY) t++; // primo slot vuoto
        while (c->scaffali[source].slots[s] != EMPTY) s++; // item da spostare

        c->scaffali[target].slots[t] = c->scaffali[source].slots[s];
        c->scaffali[source].slots[s] = EMPTY;
    }
    return 1;
}
