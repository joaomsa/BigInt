#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#ifndef BIGNUM_H
#define BIGNUM_H
#include "bignum.h"
#endif

void num_read(list_t *num)
{
    int c;

    /* Ignore non numbers */
    while (!(isdigit((c = getchar()))) && c != '-');
    if (c == '-')
        num->head->item = 1;
    else
        list_insert(num, 0, c - '0');
    while (isdigit((c = getchar())))
        list_insert(num, 0, c - '0');
}

int num_cmp(list_t *numA, list_t *numB)
{
    int i, retVal;
    node_t *auxA, *auxB;
    /* Compare signs. */
    if (numA->head->item == 1 && numB->head->item == 1){
        numA->head->item = 0;
        numB->head->item = 0;
        retVal = num_cmp(numA, numB);
        numA->head->item = 1;
        numB->head->item = 1;
        return -retVal;
    }
    if (numA->head->item == 0 && numB->head->item == 1)
        return 1;
    if (numA->head->item == 1 && numB->head->item == 0)
        return -1;

    /* Compare length. */
    if (numA->len > numB->len)
        return 1;
    if (numA->len < numB->len)
        return -1;

    /* Compare numbers. */
    auxA = numA->tail;
    auxB = numB->tail;
    for (i = 0; i < numA->len; i++){
        if (auxA->item > auxB->item)
            return 1;
        if (auxA->item < auxB->item)
            return -1;
        auxA = auxA->prev;
        auxB = auxB->prev;
    }
    /* They're the same */
    return 0;
}

void num_add_nat(list_t *numAns, list_t *numA, list_t *numB)
{
    int maxdigits, mindigits, i, sum;
    node_t *auxAns, *auxA, *auxB;

    /* Ensure A is always longer than B */
    if (numA->len >= numB->len){
        maxdigits = numA->len;
        mindigits = numB->len;
    }
    else {
        num_add_nat(numAns, numB, numA);
        return;
    }

    list_append(numAns, 0);
    auxAns = numAns->head->next;
    auxA = numA->head->next;
    auxB = numB->head->next;
    for (i = 0; i < maxdigits; i++){
        if (i < mindigits){
            sum = auxAns->item + auxA->item + auxB->item;
            auxA = auxA->next;
            auxB = auxB->next;
        }
        else {
            sum = auxAns->item + auxA->item;
            auxA = auxA->next;
        }

        /* Carry one. */
        if (sum >= 10){
            sum -= 10;
            list_append(numAns, 1);
        }
        else
            list_append(numAns, 0);

        auxAns->item = sum;
        auxAns = auxAns->next;
    }
    /* Remove trailing zeros. */
    while (numAns->tail->item == 0 && numAns->len > 1)
        free(list_pop(numAns, numAns->len - 1));
}

void num_sub_nat(list_t *numAns, list_t *numA, list_t *numB)
{
    int i, dif;
    node_t *auxAns, *auxA, *auxB;

    list_append(numAns, 0);
    auxAns = numAns->head->next;
    auxA = numA->head->next;
    auxB = numB->head->next;
    for (i = 0; i < numA->len; i++){
        if (i < numB->len){
            dif = auxAns->item + auxA->item - auxB->item;
            auxA = auxA->next;
            auxB = auxB->next;
        }
        else {
            dif = auxAns->item + auxA->item;
            auxA = auxA->next;
        }

        /* Borrow one. */
        if (dif < 0){
            dif += 10;
            list_append(numAns, -1);
        }
        else
            list_append(numAns, 0);

        auxAns->item = dif;
        auxAns = auxAns->next;
    }
    /* Remove trailing zeros. */
    while (numAns->tail->item == 0 && numAns->len > 1)
        free(list_pop(numAns, numAns->len - 1));
}

void num_add(list_t *numAns, list_t *numA, list_t *numB)
{
    int signCount = 0, signA, signB;

    signCount += numA->head->item;
    signCount += numB->head->item;
    switch (signCount) {
        case 0: 
            num_add_nat(numAns, numA, numB);
            return;
        case 1: 
            signA = numA->head->item;
            signB = numB->head->item;
            numA->head->item = 0;
            numB->head->item = 0;

            if (num_cmp(numA, numB) >= 0){
                num_sub_nat(numAns, numA, numB);
                numAns->head->item = signA;
            }
            else {
                num_sub_nat(numAns, numB, numA);
                numAns->head->item = signB;
            }

            numA->head->item = signA;
            numB->head->item = signB;
            return;
        case 2: 
            num_add_nat(numAns, numA, numB);
            numAns->head->item = 1;
            return;
    }

}

void num_sub(list_t *numAns, list_t *numA, list_t *numB)
{
    int signB;
    signB = numB->head->item;
    /* Toggle sign */
    numB->head->item ^= 1;
    num_add(numAns, numA, numB);
    numB->head->item = signB;
}