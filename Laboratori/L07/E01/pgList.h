#ifndef PGLIST_H_DEFINED
#define PGLIST_H_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pg.h"

/// @brief ADT di prima classe collezione di personaggi
typedef struct pgList_s *pgList;

typedef struct node *link;

/// @brief Inizializza lista di personaggi
/// @return
pgList list_init();

link newNode(pg pers, link next);

/// @brief Dealloca lista di personaggi
/// @param pgList
void pgList_free(pgList list);

/// @brief Leggi lista di personaggi da file
/// @param fp
/// @param pgList
void pgList_read(FILE *fp, pgList list);

/// @brief
/// @param fp
/// @param pgList
/// @param invArray
void pgList_print(FILE *fp, pgList list, invArray invArray);

/// @brief Inserisci un nuovo personaggio nella lista
/// @param pgList
/// @param pg
void pgList_insert(pgList list, pg pg);

/// @brief Rimuovi un personaggio dalla lista
/// @param pgList
/// @param cod
void pgList_remove(pgList list, char *cod);

/// @brief Cerca personaggio per codice
/// @param pgList
/// @param cod
/// @return Puntatore al personaggio o NULL se non trovato
pg *pgList_searchByCode(pgList list, char *cod);

/* Si possono aggiungere altre funzioni se ritenute necessarie */

#endif
