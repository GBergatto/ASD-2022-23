#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "st.h"
#include "path.h"

#define MAXLEN 30

typedef struct s_graph *Graph;

Graph GRAPHinit(int V);
Graph GRAPHload(FILE *fp);
void GRAPHinsertEdge(Graph g, int id1, int id2, int wt);

// problema di verifica
Path GRAPHpathLoad(Graph g, FILE *fp);
int GRAPHpathCheck(Graph g, Path p, int M, int PF);

// problema di ricerca
Path GRAPHpathBest(Graph g, int M, int PF);
void GRAPHpathBestR(Graph g, int M, int PF, int pos, Path sol, Path bestSol);
