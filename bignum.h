#ifndef LIST_H
#define LIST_H
#include "list.h"
#endif

void num_read(list_t *num);

/* 0 if equal, -1 if A < B and 1 if A > B */
int num_cmp(list_t *numA, list_t *numB);

void num_add_nat(list_t *numAns, list_t *numA, list_t *numB);

void num_add(list_t *numAns, list_t *numA, list_t *numB);
