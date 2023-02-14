#include "graph.h"

struct s_graph {
    int V;
    ST st;
    int** mat; // matrice delle adiacenze
};

Graph GRAPHload(FILE *f) {
    int S, depth, tesoro, monete, wt, id1, id2;
    Graph g;
    char name1[MAXLEN], name2[MAXLEN];
    Stanza room;

    fscanf(f, "%d", &S);
    g = GRAPHinit(S);

    // leggo e aggiungo a ST tutte le stanze
    for (int i=0; i<g->V; i++) {
        fscanf(f, "%s %d %d %d", name1, &depth, &tesoro, &monete);
        strcpy(room.nome, name1);
        room.prof = depth;
        room.monete = monete;
        room.tesoro = tesoro;
        STinsert(g->st, room, i);
    }

    // leggo tutti i cunicoli
    while (fscanf(f, "%s %s %d", name1, name2, &wt)==3) {
        id1 = STsearch(g->st, name1);
        id2 = STsearch(g->st, name1);
        if (id1 != -1 && id2 != -2)
            GRAPHinsertEdge(g, id1, id2, wt);
    }

    return g;
}

// PROBLEMA DI VERIFICA

Path GRAPHpathLoad(Graph g, FILE *fp) {
    int n = 0, id;
    char name[MAXLEN];
    Path p = PATHinit(g->V);

    while (fscanf(fp, "%s", name)==1) {
        id = STsearch(g->st, name);
        PATHinsert(p, id);
    }
    return p;
}

int GRAPHpathCheck(Graph g, Path p, int M, int PF) {
    int n, id1, id2, trap, tot, maxDelta;
    Stanza room1, room2;

    id1 = PATHgetRoomId(p, 0);
    room1 = STgetFromId(g->st, id1);
    tot = room1.monete;
    maxDelta = room1.tesoro - room1.monete;

    for (int i=1; i<n; i++) {
        id2 = PATHgetRoomId(p, i);
        trap = g->mat[id1][id2];
        if (trap == -1)
            return 0; // non esiste l'arco id1->id2

        room2 = STgetFromId(g->st, id2);
        tot += room2.monete;
        if (room2.tesoro - room2.monete > maxDelta)
            maxDelta = room1.tesoro - room1.monete;

        M--;
        PF -= trap;

        if (M==0 || PF ==0 || room2.prof == 0) {
            if (room2.prof >= 3) return 0; // no soccorso

            tot += maxDelta; // prendo il tesoro al posto delle monete
            // rimuovo il costo dei soccorritori arrotondato per eccesso
            if (room2.prof == 1)
                tot = tot - (tot+2)/3;
            else if (room2.prof == 2)
                tot = tot - (tot+1)/2;
            PATHsetRicchezza(p, tot);
            return 1;
        }
        id1 = id2;
        room1 = room2;
    }
    return 0;
}

// PROBLEMA DI RICERCA

Path GRAPHpathBest(Graph g, int M, int PF) {
    int maxVal = 0;
    Path sol = PATHinit(g->V);
    Path bestSol = PATHinit(g->V);

    GRAPHpathBestR(g, M, PF, 0, sol, bestSol);
    return bestSol;
}

// principio di moltiplicazione
void GRAPHpathBestR(Graph g, int M, int PF, int pos, Path sol, Path bestSol) {
    int res;
    Stanza room;

    room = STgetFromId(g->st, PATHgetRoomId(sol, pos));
    if (M==0 || PF ==0 || room.prof == 0) {
        // controllo il percorso e ne calcolo le ricchezze (salvate in sol)
        res = GRAPHpathCheck(g, sol, M, PF);
        if (res) {
            if (PATHgetRicchezza(sol) > PATHgetRicchezza(bestSol))
                PATHcpy(bestSol, sol); // aggiorno soluzione migliore
        }
        return;
    }

    for (int i=0; i < g->V; i++) {
        // se esite l'arco tra il nodo corrente e i
        if (g->mat[PATHgetRoomId(sol, pos)][i] > -1) {
            PATHinsert(sol, i);
            GRAPHpathBestR(g, M, PF, pos+1, sol, bestSol);
            PATHpop(sol); // backtrack
        }
    }
    return;
}
