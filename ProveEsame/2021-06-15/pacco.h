#ifndef DEF_PACCO
#define DEF_PACCO
#define LEN 30

typedef struct s_pacco {
    char codice[LEN];
    int corridoio;
    int scaffale;
    int slot;
} Pacco;

Pacco PACCOsetVoid();
int PACCOisVoid(Pacco p);

#endif // !DEF_PACCO
