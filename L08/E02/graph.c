#include "graph.h"

#define DBG 0

struct node {
    int v;  // indice, non nome (che ricavo dalla ST)
    int wt;
    link next;
};

struct graph {
    int V;       // numero vertici
    int E;       // numero archi
    int **madj;  // matrice delle adiacenze
    link *ladj;  // lista delle adiacenze
    link z;      // sentinella (per lista adj)
    ST tab;      // tabella di simboli
};

static void GRAPHinsertEdgeMat(Graph g, int id1, int id2, int wt) {
    // grafo non orientato => matrice delle adiacenze simmetrica
    g->madj[id1][id2] = wt;
    g->madj[id2][id1] = wt;
}

Graph GRAPHinit() {
    Graph g = malloc(sizeof *g);
    g->V = 0;
    g->E = 0;
    g->madj = NULL;
    g->ladj = NULL;
    g->z = newLadjEdge(-1, -1, NULL);
    g->tab = STinit(MAX_V);
    return g;
}

static int **MATRIXinit(int r, int c, int val) {
    int **t = malloc(r * sizeof(int *));
    for (int i = 0; i < r; i++)
        t[i] = malloc(c * sizeof(int));

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            t[i][j] = val;
    return t;
}

void GRAPHfree(Graph g) {
    for (int i = 0; i < g->V; i++)
        free(g->madj[i]);
    free(g->madj);
    STfree(g->tab);
    free(g);
}

Graph GRAPHload(FILE *f) {
    int V, E = 0;
    Graph g = GRAPHinit();
    Edge *archi = (Edge *)malloc(21 * sizeof(Edge));

    // leggo il file salvando tutti i nodi (unici) in ordine alfabetico
    while (fscanf(f, "%s%s%s%s %d", archi[E].v1.id, archi[E].v1.net, archi[E].v2.id, archi[E].v2.net, &(archi[E].wt)) == 5) {
        // aggiungo i vertici alla ST se non sono già presenti
        if (STsearchByName(g->tab, archi[E].v1.id) == -1) {
            STinsert(g->tab, archi[E].v1);
        }
        if (STsearchByName(g->tab, archi[E].v2.id) == -1) {
            STinsert(g->tab, archi[E].v2);
        }
        E++;
    }
    g->V = STcount(g->tab);
    printf("%d\n", g->V);
    STprint(g->tab);

    // ho tutti gli archi e i nodi in ordine alfabetico
    // creo matrice delle adiacenze
    g->madj = MATRIXinit(g->V, g->V, -1);
    int id1, id2;
    for (int i = 0; i < E; i++) {
        // aggiungo gli archi
        id1 = STsearchByName(g->tab, archi[i].v1.id);
        id2 = STsearchByName(g->tab, archi[i].v2.id);

        GRAPHinsertEdgeMat(g, id1, id2, archi[i].wt);
    }

#if DBG
    // print mat adj
    for (int i = 0; i < g->V; i++) {
        for (int j = 0; j < g->V; j++)
            printf("%3d ", g->madj[i][j]);
        printf("\n");
    }
#endif

    free(archi);
    return g;
}

static link newLadjEdge(int v, int wt, link next) {
    link x = malloc(sizeof *x);
    x->v = v;
    x->wt = wt;
    x->next = next;
    return x;
}

int GRAPHgenerateListAdj(Graph g) {
    g->ladj = (link *)malloc(g->V * sizeof(link));
    // punto tutti i nodi alla sentinella z
    for (int i = 0; i < g->V; i++)
        g->ladj[i] = g->z;

    // itero sulla matrice delle adiacenze
    for (int i = 0; i < g->V; i++) {
        // mat adj simmetrica -> itero solo sulla triangolare inferiore
        for (int j = 0; j <= i; j++) {
            if (g->madj[i][j] != -1) {
                // aggiungo in testa
                g->ladj[i] = newLadjEdge(j, g->madj[i][j], g->ladj[i]);
                g->ladj[j] = newLadjEdge(i, g->madj[i][j], g->ladj[j]);  // grafo non orientato
            }
        }
    }

    for (int i = 0; i < g->V; i++) {
        Vert partenza, arrivo;

        partenza = STsearchByIndex(g->tab, i);
        printf("%s (%s):\n", partenza.id, partenza.net);
        for (link t = g->ladj[i]; t != g->z; t = t->next) {
            arrivo = STsearchByIndex(g->tab, t->v);
            printf("   %s (%s) %d\n", arrivo.id, arrivo.net, t->wt);
        }
    }
}

Graph GRAPHlistNodes(Graph g, FILE *out) {
    for (int i = 0; i < g->V; i++) {
        Vert v = STsearchByIndex(g->tab, i);
        fprintf(out, "%s (%s)\n", v.id, v.net);
        for (int j = 0; j < g->V; j++) {
            if (g->madj[i][j] != -1) {
                Vert w = STsearchByIndex(g->tab, j);
                fprintf(out, "  -> %s (%s)\n", w.id, w.net);
            }
        }
    }
}

int GRAPHcheckAdjacencyMat(Graph g, char n1[], char n2[], char n3[]) {
    int id1, id2, id3;
    id1 = STsearchByName(g->tab, n1);
    id2 = STsearchByName(g->tab, n2);
    id3 = STsearchByName(g->tab, n3);

    if (id1 == -1 || id2 == -1 || id3 == -1)
        return -1;  // vertice non trovato

    // per formare un sottografo completo, ogni nodo deve essere connesso agli altri due
    if (g->madj[id1][id2] != -1 && g->madj[id1][id3] != -1 && g->madj[id3][id2] != -1)
        return 1;  // adiacenti
    return 0;      // non adiacenti
}

int GRAPHcheckAdjacencyList(Graph g, char n1[], char n2[], char n3[]) {
    if (g->ladj == NULL) return -2;  // lista non inizializzata

    int id1, id2, id3;
    id1 = STsearchByName(g->tab, n1);
    id2 = STsearchByName(g->tab, n2);
    id3 = STsearchByName(g->tab, n3);

    if (id1 == -1 || id2 == -1 || id3 == -1)
        return -1;  // vertice non trovato

    int f12 = 0, f13 = 0, f23 = 0;
    for (link t = g->ladj[id1]; t != g->z; t = t->next) {
        if (t->v == id2) f12 = 1;
        if (t->v == id3) f13 = 1;
    }
    for (link t = g->ladj[id2]; t != g->z; t = t->next) {
        if (t->v == id3) f23 = 1;
    }

    return (f12 && f13 && f23);
}