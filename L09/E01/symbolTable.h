#ifndef ST_DEFINED
#define ST_DEFINED

// ADT Tabella di simboli implementata con vettore non ordinato

typedef struct symboltable *ST;

ST STinit(int maxN);
int STgetIndex(ST st, char *name);
char *STgetName(ST st, int index);
void STinsert(ST st, char *name);
void STfree(ST st);

#endif