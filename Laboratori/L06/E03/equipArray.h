#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQUIP_SLOT 2

#include "invArray.h"

typedef struct {
    int inUse;
    inv **array;  // puntatori agli oggetti nell'inventario
} * equipArray;

equipArray equipArray_init();

void equipArray_free(equipArray eqArray);

/// @brief Numero di oggetti equipaggiati
/// @param eqArray
/// @return
int equipArray_inUse(equipArray eqArray);

/// @brief Stampa equipaggiamento su file
/// @param fp
/// @param eqArray
/// @param invArray
void equipArray_print(FILE *fp, equipArray eqArray, invArray invArray);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

/// @brief Trova un oggetto equipaggiato dal nome
/// @param eqArray
/// @param name nome dell'oggetto
/// @return Indice nell'inventario dell'oggetto
int equipArray_searchByName(equipArray eqArray, char *name);

/// @brief Aggiungi un oggetto all'equipaggiamento
/// @param equipArray
/// @param object puntatore all'oggetto da aggiungere
/// @return 0 per successo, -1 per limite di oggetti superato
int equipArray_addObject(equipArray equipArray, inv *object);

/// @brief Rimuovi oggetto equipaggiato
/// @param equipArray
/// @param index
/// @return puntatore all'oggetto rimosso
inv *equipArray_removeObject(equipArray eq, int index);

#endif
