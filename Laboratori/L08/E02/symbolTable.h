#ifndef ST_DEFINED
#define ST_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vert.h"

typedef struct symboltable *ST;

ST STinit(int maxN);
void STfree(ST st);
int STcount(ST st);
void STprint(ST st);
void STinsert(ST st, Vert val);

/// @brief Da indice a nome+rete
/// @param st
/// @param index
/// @return
Vert STsearchByIndex(ST st, int index);

/// @brief Da nome a indice
/// @param st
/// @param k
/// @return
int STsearchByName(ST st, char k[]);

#endif