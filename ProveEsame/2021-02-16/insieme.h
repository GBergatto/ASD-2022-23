#include "articolo.h"

// insieme di stringhe di lunghezza MAXLEN
typedef struct set *Set;

Set SETinit(int maxN);
void SETfree(Set s);
int SETempty(Set s);
void SETdisplay(Set s);
int SETsize(Set s);
void SETfill(Set s, char val[]);
int SETsearch(Set s, char k[]);
void SETdeleteAll(Set s); // elimina tutti gli elementi
