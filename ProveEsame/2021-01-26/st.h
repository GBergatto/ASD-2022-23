#define MAXLEN 30

typedef struct s_st *ST;

typedef struct vert {
    char nome[MAXLEN];
    int prof;
    int tesoro;
    int monete;
} Stanza;

void STinsert(ST st, Stanza room, int i);
int STsearch(ST st, char* name);
Stanza STgetFromId(ST st, int id);
