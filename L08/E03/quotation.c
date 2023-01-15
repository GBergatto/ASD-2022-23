#include "quotation.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "tempo.h"

Quotation QsetNull() {
    Quotation q;
    q.date = DATEsetNull();
    q.value = 0;
    return q;
}

typedef struct BSTnode *blink;
struct BSTnode {
    Quotation item;
    int N;
    blink l;
    blink r;
};

struct bst {
    blink root;
    blink z;
};

static blink NEW(Quotation item, blink l, blink r, int n) {
    blink x = malloc(sizeof *x);
    x->item = item;
    x->l = l;
    x->r = r;
    x->N = n;
    return x;
}

static void treeFree(blink h, blink z) {
    if (h == z) return;

    treeFree(h->l, z);
    treeFree(h->r, z);
    free(h);
}

BST BSTinit() {
    BST bst = malloc(sizeof(BST));
    bst->z = NEW(QsetNull(), NULL, NULL, 0);
    bst->root = bst->z;
    return bst;
}

void BSTfree(BST bst) {
    if (bst == NULL)
        return;
    treeFree(bst->root, bst->z);
    free(bst->z);
    free(bst);
}

static int countR(blink h, blink z) {
    if (h == z)
        return 0;
    return countR(h->l, z) + countR(h->r, z) + 1;
}

int BSTcount(BST bst) {
    return countR(bst->root, bst->z);
}

static Quotation searchR(blink h, t_data k, blink z) {
    if (h == z) return QsetNull();

    int cmp = DATEcmp(k, h->item.date);
    if (cmp == 0)
        return h->item;
    if (cmp <= 0)
        return searchR(h->l, k, z);
    return searchR(h->r, k, z);
}

Quotation BSTsearch(BST bst, t_data key) {
    return searchR(bst->root, key, bst->z);
}

static blink insertR(blink h, Quotation x, blink z) {
    if (h == z) return NEW(x, z, z, 1);

    if (DATEcmp(x.date, h->item.date) < 0)
        h->l = insertR(h->l, x, z);
    else
        h->r = insertR(h->r, x, z);
    (h->N)++;
    return h;
}

void BSTinsertLeaf(BST bst, Quotation x) {
    bst->root = insertR(bst->root, x, bst->z);
}

static void visit(blink x, blink z, int strategy) {
    if (x == z) return;

    if (strategy == preorder) printf("%d %f N=%d\n", x->item.date.day, x->item.value, x->N);
    visit(x->l, z, strategy);
    if (strategy == inorder) printf("%d %f N=%d\n", x->item.date.day, x->item.value, x->N);
    visit(x->r, z, strategy);
    if (strategy == postorder) printf("%d %f N=%d\n", x->item.date.day, x->item.value, x->N);
}

void BSTvisit(BST bst, int strategy) {
    visit(bst->root, bst->z, strategy);
}

static void findR(blink x, blink z, t_data s, t_data e, Quotation *max, Quotation *min) {
    if (x == z) return;

    findR(x->l, z, s, e, max, min);
    if (DATEcmp(x->item.date, s) >= 0 && DATEcmp(x->item.date, e) <= 0) {
        if (x->item.value > max->value) {  // aggiorna max
            max->value = x->item.value;
            max->date.day = x->item.date.day;
            max->date.month = x->item.date.month;
            max->date.year = x->item.date.year;
        }
        if (x->item.value < min->value) {  // aggiorna min
            min->value = x->item.value;
            min->date.day = x->item.date.day;
            min->date.month = x->item.date.month;
            min->date.year = x->item.date.year;
        }
    }
    findR(x->r, z, s, e, max, min);
}

void BSTfindExtremesInRange(BST bst, t_data start, t_data end, Quotation *pmax, Quotation *pmin) {
    pmax->value = 0;
    pmin->value = 9E10;

    findR(bst->root, bst->z, start, end, pmax, pmin);
}

static Quotation minR(blink h, blink z) {
    if (h == z)
        return QsetNull();
    if (h->l == z)
        return (h->item);
    return minR(h->l, z);
}

Quotation BSTmin(BST bst) {  // wrapper
    return minR(bst->root, bst->z);
}

static Quotation maxR(blink h, blink z) {
    if (h == z)
        return QsetNull();
    if (h->r == z)
        return (h->item);
    return maxR(h->r, z);
}

Quotation BSTmax(BST bst) {  // wrapper
    return maxR(bst->root, bst->z);
}

// ### Bilanciamento ###

static void pathR(blink x, blink z, int pos, int *max, int *min) {
    if (x == z) {
        if (pos > *max)
            *max = pos;
        if (pos < *min)
            *min = pos;
        return;
    }

    pathR(x->l, z, pos + 1, max, min);
    pathR(x->r, z, pos + 1, max, min);
}

int BSTgetPathsRatio(BST bst) {
    int max = 0, min = INT_MAX;
    pathR(bst->root, bst->z, 0, &max, &min);

    return (float)max / (float)min;
}

static blink rotR(blink h) {
    blink x = h->l;
    h->l = x->r;
    x->r = h;
    // aggiorno dimensione del sottoalbero
    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}

static blink rotL(blink h) {
    blink x = h->r;
    h->r = x->l;
    x->l = h;
    // aggiorno dimensione del sottoalbero
    x->N = h->N;
    h->N = 1;
    h->N += (h->l) ? h->l->N : 0;
    h->N += (h->r) ? h->r->N : 0;
    return x;
}

static blink partR(blink h, int r) {
    int t = h->l->N;

    if (t > r) {
        h->l = partR(h->l, r);
        h = rotR(h);
    }
    if (t < r) {
        h->r = partR(h->r, r - t - 1);
        h = rotL(h);
    }
    return h;
}

static blink balanceR(blink h, blink z) {
    if (h == z) return z;

    int r = (h->N + 1) / 2 - 1;
    h = partR(h, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}

void BSTbalance(BST bst) {
    printf("Prima del bilanciamento\nPreorder\n");
    BSTvisit(bst, preorder);
    printf("\nInorder\n");
    BSTvisit(bst, inorder);

    bst->root = balanceR(bst->root, bst->z);

    printf("\nDopo il bilanciamento\nPreorder\n");
    BSTvisit(bst, preorder);
    printf("\nInorder\n");
    BSTvisit(bst, inorder);
}
