#ifndef INV_H_DEFINED
#define INV_H_DEFINED

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 100

/// @brief Quasi ADT stats per oggetti e personaggi
typedef struct stat_s {
    int hp, mp, atk, def, mag, spr;
} stat;

/// @brief Quasi ADT oggetto da inventario
typedef struct inv_s {
    char nome[LEN];
    char tipo[LEN];
    stat stat;
} inv;

/// @brief Leggi statistiche da file
/// @param fp
/// @param statp
/// @return 1 se la lettura è andata a buon fine, altrimenti 0
int stat_read(FILE *fp, stat *statp);

/// @brief Stampa statistiche
/// @param fp
/// @param statp
/// @param soglia valore minimo da visualizzare
void stat_print(FILE *fp, stat *statp, int soglia);

/// @brief Leggi oggetto da file
/// @param fp
/// @param invp
void inv_read(FILE *fp, inv *invp);

/// @brief Stampa oggetto
/// @param fp
/// @param invp
void inv_print(FILE *fp, inv *invp);

/// @brief Ottiene le statistiche dell'oggetto
/// @param invp
/// @return
stat inv_getStat(inv *invp);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

/// @brief Somma due stat e salva il risultato nel puntatore alla prima
/// @param a puntatore della stat che viene aggiornata
/// @param b
void stat_sum(stat *a, stat b);

/// @brief Sottrai una stat alla prima
/// @param a puntatore della stat che viene aggiornata
/// @param b
void stat_difference(stat *a, stat b);

#endif
