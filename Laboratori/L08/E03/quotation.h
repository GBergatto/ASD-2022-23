#ifndef QUOTE_DEFINED
#define QUOTE_DEFINED

#include "tempo.h"

enum strategy { preorder,
                inorder,
                postorder };

typedef struct quotation {
    t_data date;
    float value;
} Quotation;

Quotation QsetNull();

typedef struct bst *BST;

BST BSTinit();
void BSTfree(BST bst);

int BSTcount(BST bst);
Quotation BSTsearch(BST bst, t_data key);
void BSTinsertLeaf(BST bst, Quotation x);
void BSTvisit(BST bst, int strategy);
void BSTfindExtremesInRange(BST bst, t_data start, t_data end, Quotation *max, Quotation *min);
Quotation BSTmin(BST bst);
Quotation BSTmax(BST bst);
void BSTbalance(BST bst);

/// calcola il rapporto tra il percorso più lungo e quello più corto
int BSTgetPathsRatio(BST bst);

#endif
