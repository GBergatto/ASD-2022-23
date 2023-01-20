#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct graph *GRAPH;

GRAPH GRAPHload(FILE *f);
void GRAPHfree(GRAPH g);

int GRAPHisDag(GRAPH g, int *mark, int *pre, int *post);
int GRAPHgetWeight(GRAPH g, int *mark);

void GRAPHaddEdges(GRAPH g, int *toAdd);
void GRAPHremoveEdges(GRAPH g, int *toRemove, int permanently);
void GRAPHdagMaximumPath(GRAPH g);

void GRAPHprintListAdj(GRAPH g);
void GRAPHprintEdge(GRAPH g, int index);
int GRAPHcountEdges(GRAPH g);
int GRAPHcountVert(GRAPH g);

#endif