#ifndef GRAPH_DEFINED
#define GRAPH_DEFINED

#include <stdio.h>
#include <stdlib.h>

#include "symbolTable.h"
#include "vert.h"

#define MAX_V 10  // numero di vertici

typedef struct edge {
    Vert v1;
    Vert v2;
    int wt;
} Edge;

typedef struct graph *Graph;
typedef struct node *link;

Graph GRAPHinit();
void GRAPHfree(Graph g);

static void GRAPHinsertEdgeMat(Graph g, int id1, int id2, int wt);
Graph GRAPHload(FILE *f);
Graph GRAPHlistNodes(Graph g, FILE *out);
int GRAPHgenerateListAdj(Graph g);
static link newLadjEdge(int v, int wt, link next);

/// @brief dati 3 vertici verifica se essi sono adiacenti a coppie (tramite matrice)
/// @param g
/// @param n1
/// @param n2
/// @param n3
/// @return -1 se uno dei vertici non esiste, 0 se non sono adiacenti a coppie, 1 se lo sono
int GRAPHcheckAdjacencyMat(Graph g, char n1[], char n2[], char n3[]);

/// @brief dati 3 vertici verifica se essi sono adiacenti a coppie (tramite lista)
/// @param g
/// @param n1
/// @param n2
/// @param n3
/// @return -2 se la lista non è inizializzata, -1 se uno dei vertici non esiste, 0 se non sono adiacenti a coppie, 1 se lo sono
int GRAPHcheckAdjacencyList(Graph g, char n1[], char n2[], char n3[]);

#endif