#include "inv.h"

int stat_read(FILE *fp, stat *statp) {
    int cnt = fscanf(fp, "%d%d%d %d%d%d", &(statp->hp), &(statp->mp), &(statp->atk), &(statp->def), &(statp->mag), &(statp->spr));
    return (cnt == 6);
}

void stat_print(FILE *fp, stat *statp) {
    fprintf(fp, "%d %d %d %d %d %d\n", statp->hp, statp->mp, statp->atk, statp->def, statp->mag, statp->spr);
}

void stat_sum(stat *a, stat b) {
    a->atk = a->atk + b.atk;
    a->def = a->def + b.def;
    a->hp = a->hp + b.hp;
    a->mag = a->mag + b.mag;
    a->spr = a->spr + b.spr;
    a->hp = a->hp + b.hp;
}

void stat_difference(stat *a, stat b) {
    a->atk = a->atk - b.atk;
    a->def = a->def - b.def;
    a->hp = a->hp - b.hp;
    a->mag = a->mag - b.mag;
    a->spr = a->spr - b.spr;
    a->hp = a->hp - b.hp;
}

void inv_read(FILE *fp, inv *invp) {
    fscanf(fp, "%s%s", invp->nome, invp->tipo);
    stat_read(fp, &(invp->stat));
}

void inv_print(FILE *fp, inv *invp) {
    fprintf(fp, "%s %s ", invp->nome, invp->tipo);
    stat_print(fp, &(invp->stat));
}

stat inv_getStat(inv *invp) {
    // ritorna il campo stat di un oggetto dell'inventario
    return invp->stat;
}