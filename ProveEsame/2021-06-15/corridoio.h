#include "pacco.h"

typedef struct s_corridoio *Corridoio;

Corridoio CORRIDOIOinit(int s);
void CORRIDOIOfree(Corridoio c);
int CORRIDOIOsize(Corridoio c);

// -1 libera, -2 non esiste, >=0 id del pacco contenuto
int CORRIDOIOcheckPos(Corridoio c, int scaffale, int slot);

// inserisce id alla posizione richiesta (senza controllarne la validità)
void CORRIDOIOinserisciPacco(Corridoio c, int scaf, int slot, int id);

// estrae il pacco contenuto alla posizione richiesta (senza controllarne la validità)
int CORRIDOIOestraiPacco(Corridoio c, int scaf, int slot);

// compatta due scaffali in uno (target) o restituisce -1
int CORRIDOIOcompattaScaffali(Corridoio c, int target, int source);
