// ADT di 1 classe

typedef struct s_path *Path;

Path PATHinit(int nStanze);
int PATHsize(Path p);

// aggiungi e rimuovi una stanza al fondo del vettore
void PATHinsert(Path p, int id);
void PATHpop(Path p);

// per scorrere il vettore dall'esterno dell'ADT, uso un for che chiama questa funzione
// che data la posizione pos nel vettore, restituisce l'indice corrispondente
int PATHgetRoomId(Path p, int pos);

int PATHgetRicchezza(Path p);
void PATHsetRicchezza(Path p, int tot);

void PATHcpy(Path destination, Path source);
