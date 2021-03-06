#include <stdio.h>
#include <stdlib.h>
#ifndef BIGNUM_H
#define BIGNUM_H
#include "bignum.h"
#endif

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
    int i, sum, carry, lenNumAns, lenNumA, lenNumB;
    node_t *auxAns, *auxA, *auxB;

    /* Ensure A is always longer than B */
    if (numA->len < numB->len){
        num_add_nat(numAns, numB, numA);
        return;
    }

    carry = 0;
    lenNumA = numA->len;
    lenNumB = numB->len;

    if (numAns->head->next == NULL)
        list_append(numAns, 0);
    auxAns = numAns->head->next;
    auxA = numA->head->next;
    auxB = numB->head->next;

    for (i = 0; i < lenNumA; i++){
        if (i < lenNumB){
            sum = carry + auxA->item + auxB->item;
            auxA = auxA->next;
            auxB = auxB->next;
        }
        else {
            sum = carry + auxA->item;
            auxA = auxA->next;
        }

        /* Carry one. */
        if (sum > 9){
            sum -= 10;
            carry = 1;
        }
        else
            carry = 0;
        if (auxAns->next == NULL)
            list_append(numAns, carry);
        auxAns->item = sum;
        auxAns = auxAns->next;
    }

    /* Remove leading zeros. */
    lenNumAns = i + carry;
    while(numAns->len > lenNumAns)
        free(list_pop(numAns, numAns->len - 1));
}

void num_sub_nat(list_t *numAns, list_t *numA, list_t *numB)
{
    int i, dif, borrow, lenNumAns, lenNumA, lenNumB;
    node_t *auxAns, *auxA, *auxB;

    borrow = 0;
    lenNumA = numA->len;
    lenNumB = numB->len;

    if (numAns->head->next == NULL)
        list_append(numAns, 0);
    auxAns = numAns->head->next;
    auxA = numA->head->next;
    auxB = numB->head->next;

    for (i = 0; i < lenNumA; i++){
        if (i < lenNumB){
            dif = borrow + auxA->item - auxB->item;
            auxA = auxA->next;
            auxB = auxB->next;
        }
        else {
            dif = borrow + auxA->item;
            auxA = auxA->next;
        }

        /* Borrow one. */
        if (dif < 0){
            dif += 10;
            borrow = -1;
        }
        else
            borrow = 0;
        if (auxAns->next == NULL)
            list_append(numAns, borrow);
        auxAns->item = dif;
        auxAns = auxAns->next;
    }

    /* Remove leading zeros. */
    lenNumAns = i;
    while(numAns->len > lenNumAns)
        free(list_pop(numAns, numAns->len - 1));
    while(numAns->tail->item == 0 && numAns->len > 1)
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

            if (numAns != numA)
                numA->head->item = signA;
            if (numAns != numB)
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
    if (numAns != numB)
        numB->head->item = signB;
}

/* Long multiplication. */
void num_mul_long(list_t *numAns, list_t *numA, list_t *numB)
{
    int i, j, product;
    div_t carry;
    list_t *numTmp, *numAcc;
    node_t *auxA, *auxB, *auxTmp;

    /* Ensure numA is always larger. */
    if (numA->len < numB->len){
        num_mul_long(numAns, numB, numA);
        return;
    }
    /* In the event one of the factors is also the product make a copy of it so
     * it doesn't mess up during multiplication. */
    if (numAns == numA || numAns == numB)
        numAcc = list_init();
    else
        numAcc = numAns;

    numTmp = list_init();
    auxB = numB->head->next;

    for (i = 0; i < numB->len; i++){
        list_append(numTmp, 0);
        auxA = numA->head->next;
        auxTmp = numTmp->head->next;

        /* Calculate parial product. */
        for (j = 0; j < numA->len; j++){
            product = auxA->item * auxB->item + auxTmp->item;
            carry = div(product, 10);
            auxTmp->item = carry.rem;
            list_append(numTmp, carry.quot);
            auxA = auxA->next;
            auxTmp = auxTmp->next;
        }

        /* Remove leading zeros. */
        while(numTmp->tail->item == 0 && numTmp->len > 1)
            free(list_pop(numTmp, numTmp->len - 1));

        /* Add zeros to the right. */
        for (j = 0; j < i; j++)
            list_insert(numTmp, 0, 0);

        /* Accumulate partial products. */
        num_add_nat(numAcc, numAcc, numTmp);
        auxB = auxB->next;
        list_empty(numTmp);
    }

    /* Copy the number back */
    if (numAns == numA || numAns == numB){
        list_empty(numAns);
        list_append(numAns, 0);
        num_add_nat(numAns, numAns, numAcc);
        list_free(numAcc);
    }

    list_free(numTmp);
}

/* Karatsuba multiplication. See documentation for explanation. */
void num_mul_krt(list_t *numAns, list_t *numA, list_t *numB)
{
    int i, k;
    list_t *x0, *x1, *xt,
           *y0, *y1, *yt,
           *z0, *z1, *z2;
    node_t *aux;

    if (numA->len <= THRESHOLD || numB->len <= THRESHOLD)
        num_mul_long(numAns, numA, numB);
    else {
        if (numA->len > numB->len)
            k = numB->len / 2;
        else
            k = numA->len / 2;

        x0 = list_init(); x1 = list_init(); xt = list_init();
        y0 = list_init(); y1 = list_init(); yt = list_init();
        z0 = list_init(); z1 = list_init(); z2 = list_init();

        aux = numA->head->next;
        x0->len = k;
        x0->head->next = aux;
        for (i = 0; i < k; i++){
            x0->tail = aux;
            aux = aux->next;
        }
        x1->len = numA->len - k;
        x1->head->next = aux;
        for (i = 0; i < numA->len - k; i++){
            x1->tail = aux;
            aux = aux->next;
        }

        aux = numB->head->next;
        y0->len = k;
        y0->head->next = aux;
        for (i = 0; i < k; i++){
            y0->tail = aux;
            aux = aux->next;
        }
        y1->len = numB->len - k;
        y1->head->next = aux;
        for (i = 0; i < numB->len - k; i++){
            y1->tail = aux;
            aux = aux->next;
        }

        num_mul_krt(z0, x0, y0);
        num_mul_krt(z2, x1, y1);

        num_add_nat(xt, x1, x0);
        num_add_nat(yt, y1, y0);
        num_mul_krt(z1, xt, yt);
        num_sub_nat(z1, z1, z2);
        num_sub_nat(z1, z1, z0);

        /* Multiply by powers of 10. */
        for (i = 0; i < k; i++)
            list_insert(z1, 0, 0);
        for (i = 0; i < 2 * k; i++)
            list_insert(z2, 0, 0);

        num_add_nat(numAns, z0, z1);
        num_add_nat(numAns, numAns, z2);

        /* Clean up. */
        x0->len = 0; x1->len = 0;
        y0->len = 0; y1->len = 0;
        list_free(x0); list_free(x1); list_free(xt);
        list_free(y0); list_free(y1); list_free(yt);
        list_free(z0); list_free(z1); list_free(z2);
    }
}

void num_mul(list_t *numAns, list_t *numA, list_t *numB)
{
    int signCount;

    signCount = 0;
    signCount += numA->head->item;
    signCount += numB->head->item;

    /*
       num_mul_long(numAns, numA, numB);
       */
    num_mul_krt(numAns, numA, numB);


    switch (signCount) {
        case 1:
            numAns->head->item = 1;
            return;
        default:
            numAns->head->item = 0;
            return;
    }
}

/* Long division. */
void num_div_long(list_t *numAns, list_t *numRem, list_t *numA, list_t *numB)
{
    int i, headDig, quotDig;
    list_t *numAnsTmp, *numATmp;

    /* Backup numA. */
    numATmp = list_init();
    list_copy(numATmp, numA);

    /* If numAns is either numA or numB. */
    if (numAns == numA || numAns == numB)
        numAnsTmp = list_init();
    else
        numAnsTmp = numAns;

    /* Prepare to work backwards through list. */
    numATmp->len = 0;
    numATmp->head = numATmp->tail;

    /* Long division. */
    for (i = 0; i < numA->len; i++){
        /* Get previous most significant digit of numA. */
        numATmp->head = numATmp->head->prev;
        numATmp->len++;
        headDig = numATmp->head->item;
        numATmp->head->item = 0;

        /* Clear remainder. */
        list_empty(numRem);
        list_append(numRem, 0);

        /* Find quotient of partial division. */
        for (quotDig = 0; num_cmp(numATmp, numRem) != -1; quotDig++)
            num_add_nat(numRem, numRem, numB);
        quotDig--;
        list_insert(numAnsTmp, 0, quotDig);

        /* Set numRem to actual remainder. */
        if (quotDig == 0){
            list_copy(numRem, numATmp);
        }
        else {
            num_sub_nat(numRem, numRem, numB);
            num_sub_nat(numRem, numATmp, numRem);
        }

        /* Prepare for next partial division. */
        list_empty(numATmp);
        if (numRem->tail->item != 0)
            list_concat(numATmp, numRem);
        numATmp->head->item = headDig;
    }

    /* Remove leading zeros from answer. */
    while(numAnsTmp->tail->item == 0 && numAnsTmp->len > 1)
        free(list_pop(numAnsTmp, numAnsTmp->len - 1));

    /* Cleanup. */
    if (numAns == numA || numAns == numB){
        list_copy(numAns, numAnsTmp);
        list_free(numAnsTmp);
    }

    list_free(numATmp);
}

void num_div(list_t *numAns, list_t *numA, list_t *numB)
{
    int signCount = 0, signA, signB;
    list_t *numRem;


    if (numB->tail->item == 0 && numB->len == 1){
        fprintf(stderr, "Dividing by 0\n");
        abort();
    }


    /* Stores remainder throughout division. */
    numRem = list_init();

    signCount += numA->head->item;
    signCount += numB->head->item;

    /* Backup signs since num_div_long() needs both numbers to be positive. */
    signA = numA->head->item;
    signB = numB->head->item;
    numA->head->item = 0;
    numB->head->item = 0;

    num_div_long(numAns, numRem, numA, numB);

    numA->head->item = signA;
    numB->head->item = signB;

    list_free(numRem);

    /* Set correct sign of answer. */
    switch (signCount) {
        case 1:
            numAns->head->item = 1;
            return;
        default:
            numAns->head->item = 0;
            return;
    }
}

/* Factorial. */
void num_fct(list_t *numAns, list_t *numA)
{
    list_t *num1, *numATmp;

    if (numA->head->item == 1){
        fprintf(stderr, "Factorial of negative number\n");
        abort();
    }

    num1 = list_init();
    list_append(num1, 1);

    numATmp = list_init();
    list_copy(numATmp, numA);

    /* 0! = 1 */
    if (numA->tail->item == 0 && numA->len == 1){
        list_empty(numAns);
        list_append(numAns, 1);
    } else {
        list_copy(numAns, numA);
        while (num_cmp(numATmp, num1) == 1){
            num_sub_nat(numATmp, numATmp, num1);
            num_mul(numAns, numAns, numATmp);
        }
    }

    list_free(num1);
    list_free(numATmp);
}
