#include "graph.h"

#include <limits.h>
#include <stdlib.h>

#include "symbolTable.h"

#define DBG 0
#define MAX_LEN 31

typedef struct edge_s {
    int v1;
    int v2;
    int weight;
} Edge;

typedef struct node *link;
struct node {
    int index;
    int weight;
    link next;
};

struct graph {
    int V;        // numero vertici
    int E;        // numero archi
    Edge *edges;  // lista degli archi
    link *ladj;   // lista delle adiacenze
    link z;       // sentinella
    ST st;        // tabella di simboli
};

static link NEW(int v, int wt, link next);
static int dfsR(GRAPH g, Edge e, int *time, int *pre, int *post);
static void TSdfsR(GRAPH g, int v, int *ts, int *pre, int *time);

void GRAPHprintListAdj(GRAPH g) {
    link t;
    for (int i = 0; i < g->V; i++) {
        printf("[%s] -> ", STgetName(g->st, i));
        for (t = g->ladj[i]; t != g->z; t = t->next)
            printf("%s -> ", STgetName(g->st, t->index));
        printf("\n");
    }
    printf("\n");
}

void GRAPHprintEdge(GRAPH g, int index) {
    printf("%s-%s (%2d)", STgetName(g->st, g->edges[index].v1), STgetName(g->st, g->edges[index].v2), g->edges[index].weight);
}

int GRAPHcountEdges(GRAPH g) {
    return g->E;
}

int GRAPHcountVert(GRAPH g) {
    return g->V;
}

static link NEW(int v, int wt, link next) {
    link x = malloc(sizeof *x);
    x->index = v;
    x->weight = wt;
    x->next = next;
    return x;
}

GRAPH GRAPHload(FILE *f) {
    char id[MAX_LEN], id1[MAX_LEN], id2[MAX_LEN];
    Edge edge;

    GRAPH g = malloc(sizeof(*g));

    fscanf(f, "%d", &(g->V));

    g->st = STinit(g->V);
    g->z = NEW(-1, -1, NULL);
    g->ladj = malloc(g->V * sizeof(link));

    g->edges = malloc(2 * g->V * sizeof(Edge));
    g->E = 0;

    for (int i = 0; i < g->V; i++) {
        fscanf(f, "%s", id);
        STinsert(g->st, id);

        // inizializzo la lista delle adiacenze ai nodi sentinella
        g->ladj[i] = g->z;
    }

    // leggo gli archi e li aggiungo alla lista "edges"
    while (fscanf(f, "%s %s %d", id1, id2, &(edge.weight)) == 3) {
        edge.v1 = STgetIndex(g->st, id1);
        edge.v2 = STgetIndex(g->st, id2);
        g->edges[g->E++] = edge;
    }

    // costruisco lista delle adiacenze
    for (int i = 0; i < g->E; i++) {
        g->ladj[g->edges[i].v1] = NEW(g->edges[i].v2, g->edges[i].weight, g->ladj[g->edges[i].v1]);
    }

    return g;
}

void GRAPHfree(GRAPH g) {
    link t, tmp;
    for (int i = 0; i < g->V; i++) {
        for (t = g->ladj[i]; t != g->z; t = tmp) {
            tmp = t->next;  // per poter liberare t senza perdere il puntatore al successivo
            free(t);
        }
    }

    STfree(g->st);
    free(g->ladj);
    free(g->z);
    free(g->edges);
    free(g);
}

int GRAPHisDag(GRAPH g, int *toRemove, int *pre, int *post) {
    int time = 0, isDag = 1;
    Edge edge;

    for (int v = 0; v < g->V; v++) {
        pre[v] = -1;
        post[v] = -1;
    }

    // elimino temporaneamente gli archi selezionati
    GRAPHremoveEdges(g, toRemove, 0);
    // uso DFS per cercare archi back
    for (int v = 0; v < g->V && isDag; v++) {
        if (pre[v] == -1) {
            edge.v1 = edge.v2 = v;  // arco fittizio
            if (dfsR(g, edge, &time, pre, post))
                isDag = 0;
        }
    }
    // aggiungo nuovamente gli archi selezionati
    GRAPHaddEdges(g, toRemove);
    return isDag;
}

// depth first search con ricerca di archi back (return 1 se ne trova)
static int dfsR(GRAPH g, Edge e, int *time, int *pre, int *post) {
    link t;
    int v, w = e.v2;
    Edge x;

    pre[w] = (*time)++;
    for (t = g->ladj[w]; t != g->z; t = t->next) {
        if (pre[t->index] == -1) {
            x.v1 = w;
            x.v2 = t->index;
            if (dfsR(g, x, time, pre, post))
                return 1;
        } else {
            v = t->index;
            if (post[v] == -1) {  // arco back => non è un DAG
                return 1;
            }
        }
    }
    post[w] = (*time)++;
    return 0;  // nessun arco back trovato => DAG
}

// calcola il peso del sottoinsieme da rimuovere
int GRAPHgetWeight(GRAPH g, int *mark) {
    int sum = 0, i;
    for (i = 0; i < g->E; i++) {
        if (mark[i])
            sum += g->edges[i].weight;
    }
    return sum;
}

void GRAPHaddEdges(GRAPH g, int *toAdd) {
    for (int i = 0; i < g->E; i++) {
        if (toAdd[i]) {
            g->ladj[g->edges[i].v1] = NEW(g->edges[i].v2, g->edges[i].weight, g->ladj[g->edges[i].v1]);
        }
    }
}

/// @brief Elimina gli archi selezionati
/// @param permanently 0 per eliminarli solo dalla lista delle adiacenze, 1 per eliminarli anche dalla lista di archi
void GRAPHremoveEdges(GRAPH g, int *toRemove, int permanently) {
    link t, p;
    // scorro la lista degli archi
    for (int i = g->E - 1; i >= 0; i--) {
        if (toRemove[i]) {
            // cerco ed elimino l'arco dalla lista delle adiacenze
            for (t = g->ladj[g->edges[i].v1], p = g->z; t != g->z; p = t, t = t->next) {
                if (g->edges[i].v2 == t->index) {
                    if (t == g->ladj[g->edges[i].v1])
                        g->ladj[g->edges[i].v1] = t->next;
                    else
                        p->next = t->next;

                    free(t);
                    break;
                }
            }
            if (permanently) {  // elimino definitivamente gli archi selezionati (per la creazione del DAG)
                g->E--;
                // traslo tutti gli elementi sucessivi di una posizione
                for (int j = i; j < g->E; j++) {
                    g->edges[j] = g->edges[j + 1];
                }
            }
        }
    }
}

// Ordinamento topologico inverso
static void TSdfsR(GRAPH g, int v, int *ts, int *pre, int *time) {
    pre[v] = 0;
    for (link t = g->ladj[v]; t != g->z; t = t->next)
        if (pre[t->index] == -1)
            TSdfsR(g, t->index, ts, pre, time);
    ts[(*time)++] = v;
}

void GRAPHdagMaximumPath(GRAPH g) {
    int time = 0, *pre, *ts, *d, *p, *isSource;
    link t;

    pre = malloc(g->V * sizeof(int));
    ts = malloc(g->V * sizeof(int));
    d = malloc(g->V * sizeof(int));
    p = malloc(g->V * sizeof(int));
    isSource = malloc(g->V * sizeof(int));

    for (int i = 0; i < g->V; i++) {
        pre[i] = -1;
        isSource[i] = 1;
    }

    // nodi in ordine topologico (inverso)
    for (int v = 0; v < g->V; v++)
        if (pre[v] == -1) TSdfsR(g, v, ts, pre, &time);

    printf("Nodi in ordine topologico:\n");
    for (int v = g->V - 1; v >= 0; v--)
        printf("%s ", STgetName(g->st, ts[v]));

    printf("\n\n");

    // identifico i nodi sorgente
    for (int i = 0; i < g->E; i++) {
        isSource[g->edges[i].v2] = 0;
    }

    // per ogni nodo sorgente, calcolo il percorso più lungo verso ogni altro nodo
    for (int i = 0; i < g->V; i++) {
        if (isSource[i] > 0) {
            // resetto i valori di default
            for (int j = 0; j < g->V; j++) {
                d[j] = (isSource[j] && j != i) ? INT_MAX : 0;
                p[j] = -1;
            }

            // calcolo il cammino minimo su -G (grafo con i pesi di ogni arco opposti)
            for (int v = g->V - 1; v >= 0; v--) {
                for (t = g->ladj[ts[v]]; t != g->z; t = t->next) {
                    // printf("%d > %d - %d\n", d[t->index], d[ts[v]], t->weight);
                    if (d[t->index] > d[ts[v]] - t->weight) {
                        d[t->index] = d[ts[v]] - t->weight;
                        p[t->index] = ts[v];
                    }
                }
            }

            printf("Cammini massimi a partire da %s:\n", STgetName(g->st, i));
            for (int j = 0; j < g->V; j++) {
                if (p[j] != -1) {
                    printf("%s (%d)\n", STgetName(g->st, j), abs(d[j]));
                }
            }
        }
    }

    free(pre);
    free(ts);
    free(d);
    free(p);
}
