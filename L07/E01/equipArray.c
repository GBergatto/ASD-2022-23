#include "equipArray.h"

// TODO: use index instead of pointers
// the rest should be kind of fine

struct eqArr_s {
    int inUse;
    int* array;  // vettore di indici
};

equipArray equipArray_init() {
    equipArray eq = (equipArray)malloc(sizeof(*eq));
    eq->inUse = 0;
    eq->array = (int*)malloc(EQUIP_SLOT * sizeof(int));
    return eq;
}

void equipArray_free(equipArray eqArray) {
    free(eqArray->array);
    free(eqArray);
}

int equipArray_inUse(equipArray eqArray) {
    return eqArray->inUse;
}

void equipArray_print(FILE* fp, equipArray eq, invArray inventario) {
    for (int i = 0; i < eq->inUse; i++) {
        fprintf(fp, "   %d. ", i + 1);
        inv_print(fp, invArray_getByIndex(inventario, eq->array[i]));
    }
}

int equipArray_getEquipByIndex(equipArray eqArray, int index) {
    return eqArray->array[index];
}

int equipArray_update(equipArray eq, invArray inventario, int invId) {
    // cerco l'oggetto nell'equipaggiamento
    int found = 0, eqId;
    for (int i = 0; i < eq->inUse; i++) {
        if (eq->array[i] == invId) {
            // trovato
            found = 1;
            eqId = i;
        }
    }

    if (found) {
        // slitto tutti gli oggetti dopo index di un posto verso sx
        for (int i = eqId; i < eq->inUse - 1; i++) {
            eq->array[eqId] = eq->array[eqId + 1];
        }
        eq->inUse--;
        return -1;  // oggetto rimosso

    } else {
        if (eq->inUse == EQUIP_SLOT)
            return 0;  // numero max oggetti raggiunto

        eq->array[eq->inUse] = invId;
        eq->inUse++;
        return +1;  // oggetto equipaggiato
    }
}