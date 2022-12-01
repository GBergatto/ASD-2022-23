#include "pg.h"

#include "equipArray.h"

struct pg_s {
    char cod[LEN];
    char nome[LEN];
    char classe[LEN];
    stat stats;
    equipArray equip;  // inventario
};

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
    }
}

int pg_updateEquip(pg *pgp, invArray inventario, int index) {
    int res = equipArray_update(pgp->equip, inventario, index);

    //! L'oggetto rimane anche nell'inventario
    //! Potrebbe essere equipaggiato da più persone contemporanemamente

    // calcolo le stats del personaggio aggiornate con il nuovo oggetto
    stat statOggetto = inv_getStat(invArray_getByIndex(inventario, index));
    if (res == 1) {
        stat_sum(&pgp->stats, statOggetto);
    } else if (res == -1) {
        stat_difference(&pgp->stats, statOggetto);
    }

    return res;
}
