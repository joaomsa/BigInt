#include <stdio.h>
#include <stdlib.h>
#ifndef BIGNUM_H
#define BIGNUM_H
#include "bignum.h"
#endif
#ifndef LIST_H
#define LIST_H
#include "list.h"
#endif
/* TODO 
 * Fix list pop to be faster at the end of lists
 * init an already inited list
 * fix places where list copy would be mroe aproproate
 * */
void combination(list_t *numAns, list_t *numN, list_t *numM)
{
    list_t *numA, *numB, *numC;
    numA = list_init();
    numB = list_init();
    numC = list_init();

    num_fct(numA, numN);
    num_fct(numB, numM);
    num_sub(numC, numN, numM);
    num_fct(numC, numC);
    num_mul(numB, numB, numC);
    num_div(numAns, numA, numB);

    list_free(numA);
    list_free(numB);
    list_free(numC);
}

int main()
{
    int line = 1;
    list_t *numN, *numM, *numAns, *numAcc;
    numN = list_init();
    numM = list_init();
    numAns = list_init();
    numAcc = list_init();
    list_append(numAcc, 0);

    while (1){
        num_read(numN);
        num_read(numM);

        if (numN->len == 1 && numN->tail->item == 0 && 
                numM->len == 1 && numM->tail->item == 0)
            break;

        /*
           num_div(numAns, numN, numM);
        num_fct(numAns, numN);
           */

        combination(numAns, numN, numM);
        if (line % 2 == 1)
            num_add(numAcc, numAcc, numAns);
        else
            num_sub(numAcc, numAcc, numAns);
        /*
            */

        line++;

        /*
           if (numN->head->item == 1)
           printf("-");
           list_printrev(*numN, "");
           printf("\n");
           */
        /*
           if (numM->head->item == 1)
           printf("-");
           list_printrev(*numM, "");
           printf("\n");
           */

        if (numAns->head->item == 1)
            printf("-");
        list_printrev(*numAns, "");
        printf("\n");

        if (numAcc->head->item == 1)
            printf("-");
        list_printrev(*numAcc, "");
        printf("\n");
        /*
        */

        list_empty(numN);
        list_empty(numM);
        list_empty(numAns);
    }
    list_free(numN);
    list_free(numM);
    list_free(numAns);
    list_free(numAcc);
    return 0;
}
