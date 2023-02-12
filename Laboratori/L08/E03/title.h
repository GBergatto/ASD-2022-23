#ifndef TITLE_DEFINED
#define TITLE_DEFINED

#include "quotation.h"

typedef struct title *Title;

Title TITLEsetName(Title t, char name[]);
char *TITLEgetName(Title t);
BST TITLEgetQuotations(Title t);

// implementata come linked list
typedef struct tcollection *TC;

TC TCinit();
int TCgetCount(TC tc);
int TCprint(TC tc);
Title TCsearch(TC tc, char k[]);
void TCaddTitle(TC tc, Title val);

#endif
