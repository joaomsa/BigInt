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
 * Fix add natural lenght detection like mul
 * */
int main()
{
    list_t *numN, *numM, *numAns;
    numN = list_init();
    numM = list_init();
    numAns = list_init();

    while (1){
        num_read(numN);
        num_read(numM);
        num_mul(numAns, numN, numM);

        /*
           if (numN->head->item == 1)
           printf("-");
           list_printrev(*numN, "");
           printf("\n");
           if (numM->head->item == 1)
           printf("-");
           list_printrev(*numM, "");
           printf("\n");
           */

        if (numN->len == 1 && numN->tail->item == 0 && 
                numM->len == 1 && numM->tail->item == 0)
            break;

        if (numAns->head->item == 1)
            printf("-");
        list_printrev(*numAns, "");
        printf("\n");

        list_empty(numN);
        list_empty(numM);
        list_empty(numAns);
    }
    list_free(numN);
    list_free(numM);
    list_free(numAns);
    return 0;
}
