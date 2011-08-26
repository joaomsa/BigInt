#ifndef LIST_H
#define LIST_H
#include "list.h"
#endif
/* Threshold number of digits to start using Karatsuba multiplication instead 
 * of regular multiplicaiton. */
#define THRESHOLD 2

void num_read(list_t *num);

/* 0 if equal, -1 if A < B and 1 if A > B */
int num_cmp(list_t *numA, list_t *numB);

void num_add(list_t *numAns, list_t *numA, list_t *numB);

void num_sub(list_t *numAns, list_t *numA, list_t *numB);

void num_mul_kar(list_t *numAns, list_t *numA, list_t *numB);

void num_mul(list_t *numAns, list_t *numA, list_t *numB);
