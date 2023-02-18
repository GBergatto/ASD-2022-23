#ifndef DEF_ARTICOLO
#define DEF_ARTICOLO

#include <stdio.h>

#define MAXLEN 30

typedef struct s_articolo {
    char titolo[MAXLEN];
    char relatore[MAXLEN];
    int slot;
    char argomento[MAXLEN];
} ARTICOLO;

ARTICOLO ARTICOLOsetVoid();
int ARTICOLOisVoid(ARTICOLO a);
ARTICOLO ARTICOLOscan(FILE *f);
int ARTICOLOeq(ARTICOLO a, ARTICOLO b);

#endif
