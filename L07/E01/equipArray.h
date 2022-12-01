#ifndef EQUIPARRAY_H_DEFINED
#define EQUIPARRAY_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQUIP_SLOT 2

#include "invArray.h"

// rappresento ogni oggetto equipaggiato con il suo indice all'interno dell'inventario

typedef struct eqArr_s *equipArray;
// ADT di 1 classe: il client non deve vedere i dettagli della struct

equipArray equipArray_init();

void equipArray_free(equipArray eqArray);

/// @brief Numero di oggetti equipaggiati
/// @param eqArray
/// @return
int equipArray_inUse(equipArray eqArray);

/// @brief Stampa equipaggiamento su file
/// @param fp
/// @param eqArray
/// @param inventario
void equipArray_print(FILE *fp, equipArray eqArray, invArray inventario);

/// @brief Indice nell'inventario di un oggetto
/// @param eqArray
/// @param index indice dell'oggetto nell'equipaggiamento
/// @return indice dell'oggetto nell'inventario
int equipArray_getEquipByIndex(equipArray eqArray, int index);

/// @brief Rimuovi oggetto dall'equipaggiamento se è equipaggiato, altrimenti equipaggialo
/// @param eqArr
/// @param inventario
/// @param intId indice nell'inventario dell'oggetto da aggiungere/rimuovere
/// @return -1 per oggetto rimosso, +1 per oggetto equipaggiato, 0 per limite equipaggiamento raggiunto
int equipArray_update(equipArray eq, invArray inventario, int invId);

#endif
