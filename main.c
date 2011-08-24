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
/* TODO */
int main()
{
    list_t *numN, *numM, *numAns;
    numN = list_init();
    numM = list_init();
    numAns = list_init();

    while (!((numN->len == 1 && numN->tail->item == 0) &&
                (numM->len == 1 && numM->tail->item == 0))){
        list_empty(numN);
        list_empty(numM);
        list_empty(numAns);

        num_read(numN);
        num_read(numM);
        num_sub(numAns, numN, numM);

        if (numN->head->item == 1)
            printf("-");
        list_printrev(*numN, "");
        printf("\n");
        if (numM->head->item == 1)
            printf("-");
        list_printrev(*numM, "");
        printf("\n");
        if (numAns->head->item == 1)
            printf("-");
        list_printrev(*numAns, "");
        printf("\n");
        printf("cmp:%i\n", num_cmp(numN, numM));
    }
    return 0;
}
