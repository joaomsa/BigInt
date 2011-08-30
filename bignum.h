#ifndef LIST_H
#define LIST_H
#include "list.h"
#endif
/* Threshold number of digits to start using Karatsuba multiplication instead
 * of long multiplicaiton. */
#define THRESHOLD 2

/* Compare numA and numB. Returns 0 if equal, -1 if A < B and 1 if A > B. */
int num_cmp(list_t *numA, list_t *numB);

/* Add numA and numB and save in numAns. Can handle signs. */
void num_add(list_t *numAns, list_t *numA, list_t *numB);

/* Subtract numB from numA and save in numAns. Can handle signs. */
void num_sub(list_t *numAns, list_t *numA, list_t *numB);

/* Multiply numA and numB and save in numAns. Can handle signs. */
void num_mul(list_t *numAns, list_t *numA, list_t *numB);

/* Divide numA by numB and save in numAns. Can handle signs. */
void num_div(list_t *numAns, list_t *numA, list_t *numB);

/* Calculate factorial of numA and save in numAns. Only positive integers. */
void num_fct(list_t *numAns, list_t *numA);
