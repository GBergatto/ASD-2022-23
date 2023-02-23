typedef struct s_array *Array;

Array ARRAYinit(int size);
void ARRAYfree(Array a);
int ARRAYsize(Array a);

int ARRAYgetAtIndex(Array a, int id);
void ARRAYsetAtIndex(Array a, int id, int value);

