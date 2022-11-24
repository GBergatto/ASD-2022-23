#ifndef INVARRAY_H_DEFINED
#define INVARRAY_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

/* ADT di prima classe collezione di oggetti di inventario */

/// @brief Inventario (collezione di oggetti)
typedef struct invArray_s *invArray;

invArray invArray_init();

void invArray_free(invArray array);

/// @brief Leggi array da file
/// @param fp
/// @param array
void invArray_read(FILE *fp, invArray array);

/// @brief Stampa array su file
/// @param fp
/// @param array
void invArray_print(FILE *fp, invArray array);

/// @brief Stampa un oggetto per indice
/// @param fp
/// @param array
/// @param index
void invArray_printByIndex(FILE *fp, invArray array, int index);

/// @brief Puntatore ad oggetto selezionato da indice
/// @param array
/// @param index
/// @return puntatore all'oggetto
inv *invArray_getByIndex(invArray array, int index);

/// @brief Ritorna indice di oggetto selezionato da nome
/// @param array
/// @param name
/// @return indice dell'oggetto, -1 se non trovato
int invArray_searchByName(invArray array, char *name);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

#endif
