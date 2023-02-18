#include <stdio.h>

#include "articolo.h"
#include "collezione.h"

typedef struct programma *PROGRAMMA;

PROGRAMMA PROGinit(int R, int S);
void PROGcpy(PROGRAMMA destination, PROGRAMMA source);

PROGRAMMA PROGload(FILE *f, int R, int S);
int PROGcheck(PROGRAMMA p, ARTICOLI st);

static void disp(int pos, ARTICOLI st, int *mark, PROGRAMMA sol, PROGRAMMA best, int *bScore);
int PROGgetScore(PROGRAMMA p, ARTICOLI st);
PROGRAMMA PROGgenerateBest(ARTICOLI st, int R, int S);
