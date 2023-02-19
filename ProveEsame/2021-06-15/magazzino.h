#include "pacco.h"

typedef struct s_magazzino *Magazzino;

Pacco MAGAZZINOcercaPacco(Magazzino m, char *codice);
int MAGAZZINOinserisciPacco(Magazzino m, Pacco p);
Pacco MAGAZZINOestraiPacco(Magazzino m, int corridoio, int scaffale, int slot);
int MAGAZZINOspostaPacco(Magazzino m, int c1, int sc1, int sl1, int c2, int sc2, int sl2);
