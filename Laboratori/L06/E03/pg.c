#include "pg.h"

#include "equipArray.h"

int pg_read(FILE *fp, pg *pgp) {
    int cnt = fscanf(fp, "%s%s%s", pgp->cod, pgp->nome, pgp->classe);
    int flag = stat_read(fp, &(pgp->stats));

    // inizializza inventario
    pgp->equip = equipArray_init();
    return (cnt == 3 && flag);
}

void pg_clean(pg *pgp) {
    // siccome il personaggio non è dinamico, dealloco solo l'inventario
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg *pgp, invArray invArray) {
    fprintf(fp, "%s %s %s ", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &(pgp->stats), MIN_STAT);

    if (equipArray_inUse(pgp->equip) > 0) {
        // stampa equipaggiamento e statistiche risultanti
        equipArray_print(fp, pgp->equip, invArray);
        //
    }
}

int pg_addObject(pg *pgp, invArray inventario, int index) {
    // ottengo puntatore all'oggetto dall'inventario
    inv *oggetto = invArray_getByIndex(inventario, index);

    int res = equipArray_addObject(pgp->equip, oggetto);
    //! L'oggetto rimane anche nell'inventario
    //! Potrebbe essere equipaggiato da più persone contemporanemamente

    // calcolo le stats del personaggio aggiornate con il nuovo oggetto
    stat_sum(&pgp->stats, oggetto->stat);

    return res;
}

void pg_removeObject(pg *pgp, int index) {
    // elimino l'oggetto in base al suo indice nell'equipaggiamento del personaggio
    inv *pOggetto = equipArray_removeObject(pgp->equip, index);

    // rimuovo i bonus dell'oggetto dalle stats del personaggio
    stat_difference(&pgp->stats, pOggetto->stat);
}