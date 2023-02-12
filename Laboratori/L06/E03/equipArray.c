#include "equipArray.h"

equipArray equipArray_init() {
    equipArray eq = (equipArray)malloc(sizeof(*eq));
    eq->inUse = 0;
    eq->array = (inv**)malloc(EQUIP_SLOT * sizeof(inv*));
    return eq;
}

void equipArray_free(equipArray eqArray) {
    free(eqArray->array);
    free(eqArray);
}

int equipArray_inUse(equipArray eqArray) {
    return eqArray->inUse;
}

void equipArray_print(FILE* fp, equipArray eq, invArray invArray) {
    for (int i = 0; i < eq->inUse; i++) {
        fprintf(fp, "   %d. ", i + 1);
        inv_print(fp, eq->array[i]);
    }
}

int equipArray_searchByName(equipArray eq, char* name) {
    for (int i = 0; i < eq->inUse; i++) {
        if (strcmp(eq->array[i]->nome, name) == 0)
            return i;
    }
    return -1;
}

int equipArray_addObject(equipArray eq, inv* object) {
    if (eq->inUse == EQUIP_SLOT)  // numero max oggetti raggiunto
        return -1;

    eq->array[eq->inUse] = object;
    eq->inUse++;
    return 0;
}

inv* equipArray_removeObject(equipArray eq, int index) {
    inv* p = eq->array[index];
    // slitto tutti gli oggetti dopo index di un posto verso sx
    for (int i = index; i < eq->inUse - 1; i++) {
        eq->array[index] = eq->array[index + 1];
    }
    eq->inUse--;
    return p;
}