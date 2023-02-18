#include "articolo.h"

// tabella di simboli
typedef struct collezione *ARTICOLI;

ARTICOLI ARTICOLIinit();
ARTICOLI ARTICOLIload(FILE *f);
void ARTICOLIfree(ARTICOLI st);
int ARTICOLIsize(ARTICOLI st);

void ARTICOLIinsert(ARTICOLI st, ARTICOLO val);
ARTICOLO ARTICOLIsearch(ARTICOLI st, char k[]);

// ricerca inversa  per usare per generare sol
// stesso stile dei grafi
ARTICOLO ARTICOLIsearchByIndex(ARTICOLI st, int id);
