#ifndef PG_H_DEFINED
#define PG_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "equipArray.h"
#include "inv.h"

#define MIN_STAT 1

/// @brief Quasi ADT personaggio con riferimento a equipArray di proprietà
typedef struct pg_s {
    char cod[LEN];
    char nome[LEN];
    char classe[LEN];
    stat stats;
    equipArray equip;  // inventario
} pg;

/// @brief Leggi personaggio da file
/// @param fp
/// @param pgp
/// @return 1 se la lettura è andata a buon fine, altrimenti 0
int pg_read(FILE *fp, pg *pgp);

/// @brief Dealloca personaggio con il suo inventario
/// @param pgpil
void pg_clean(pg *pgp);

/// @brief Stampa personaggio
/// @param fp
/// @param pgp
/// @param invArray
void pg_print(FILE *fp, pg *pgp, invArray invArray);

/// @brief Aggiungi l'equipaggiamento selezionato dall'inventario
/// @param pgp
/// @param invArray
int pg_addObject(pg *pgp, invArray invArray, int index);

/// @brief Rimuovi oggetto dall'equipaggiamento
/// @param pgp
/// @param inventario
/// @param index // indice
void pg_removeObject(pg *pgp, int index);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

#endif
